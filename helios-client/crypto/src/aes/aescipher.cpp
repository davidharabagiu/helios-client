#include <algorithm>
#include <memory>
#include <cassert>

#include "aes/aescipher.h"
#include "aes/tables.h"
#include "aes/utils.h"
#include "typeconversions.h"
#include "autoresetevent.h"

using namespace Aes;

const size_t   AesCipher::s_kBlockSize  = 16;
const uint64_t AesCipher::s_kBufferSize = 16 * 1024 * 1024;  // 16 MB

AesCipher::AesCipher(KeySize keySize, int numThreads)
    : m_kKeySize(keySize)
    , m_executors(safe_integral_cast<size_t>(numThreads))
{
    static_assert(s_kBufferSize % s_kBlockSize == 0);

    for (auto& executor : m_executors)
    {
        executor.reset(new Executor());
    }
}

void AesCipher::encrypt(const uint8_t* key, std::istream& in, std::ostream& out)
{
    run(key, in, out, CipherDirection::FORWARD);
}

void AesCipher::decrypt(const uint8_t* key, std::istream& in, std::ostream& out)
{
    run(key, in, out, CipherDirection::INVERSE);
}

void AesCipher::run(const uint8_t* key, std::istream& in, std::ostream& out, CipherDirection direction)
{
    size_t rounds  = numberOfRounds(m_kKeySize);
    size_t keySize = bytesInKey(m_kKeySize);

    // Generate round keys
    auto roundKeys = new uint8_t[s_kBlockSize * (rounds + 1)];
    generateRoundKeys(key, keySize, rounds, roundKeys);

    // Get input size
    in.seekg(0, std::ios::end);
    uint64_t size = static_cast<uint64_t>(in.tellg());
    in.seekg(0);

    // Events for waiting on executors
    std::vector<std::shared_ptr<AutoResetEvent>> events;

    // Mutexes for streams
    std::mutex readMutex;
    std::mutex writeMutex;

    uint64_t pos   = 0;
    uint64_t count = size / m_executors.size();

    // We need to divide the work in multiples of s_kBlockSize bytes
    if (count % s_kBlockSize != 0)
    {
        count = (count / s_kBlockSize + 1) * s_kBlockSize;
    }

    // Create works
    for (size_t executorIdx = 0; executorIdx != m_executors.size(); ++executorIdx)
    {
        // If we reach the end of the stream, there is no need to create more works
        if (pos >= size)
        {
            break;
        }

        events.push_back(std::make_shared<AutoResetEvent>());

        m_executors[executorIdx]->post(
            [this, &in, &out, &events, &readMutex, &writeMutex, rounds, roundKeys, direction](size_t idx, uint64_t pos,
                                                                                              uint64_t count) {
                work(in, readMutex, out, writeMutex, direction, rounds, roundKeys, pos, count);
                events[idx]->set();
            },
            executorIdx, pos, std::min(count, size - pos));

        pos += count;
    }

    // Wait for completion
    for (size_t executorIdx = 0; executorIdx != events.size(); ++executorIdx)
    {
        events[executorIdx]->wait();
    }

    delete[] roundKeys;
}

void AesCipher::generateRoundKeys(const uint8_t* key, size_t keySize, size_t rounds, uint8_t* roundKeys) const
{
    auto*        _key       = reinterpret_cast<const uint32_t*>(key);
    auto*        _roundKeys = reinterpret_cast<uint32_t*>(roundKeys);
    const size_t _keySize   = keySize / sizeof(uint32_t);
    uint32_t     tmp;

    std::copy_n(_key, _keySize, _roundKeys);

    for (size_t i = _keySize; i != 4 * (rounds + 1); ++i)
    {
        if (i % _keySize == 0)
        {
            tmp = _roundKeys[i - 1];
            _roundKeys[i] =
                _roundKeys[i - _keySize] ^
                ((static_cast<uint32_t>(kSbox[(tmp >> 16) & 0xff]) << 24) |
                 (static_cast<uint32_t>(kSbox[(tmp >> 8) & 0xff]) << 16) |
                 (static_cast<uint32_t>(kSbox[tmp & 0xff]) << 8) | static_cast<uint32_t>(kSbox[tmp >> 24])) ^
                kRcon[i / _keySize];
        }
        else if (_keySize > 6 && i % _keySize == 4)
        {
            tmp           = _roundKeys[i - 1];
            _roundKeys[i] = _roundKeys[i - _keySize] ^ ((static_cast<uint32_t>(kSbox[tmp >> 24]) << 24) |
                                                        (static_cast<uint32_t>(kSbox[(tmp >> 16) & 0xff]) << 16) |
                                                        (static_cast<uint32_t>(kSbox[(tmp >> 8) & 0xff]) << 8) |
                                                        static_cast<uint32_t>(kSbox[tmp & 0xff]));
        }
        else
        {
            _roundKeys[i] = _roundKeys[i - _keySize] ^ _roundKeys[i - 1];
        }
    }
}

void AesCipher::addRoundKey(uint8_t* state, const uint8_t* roundKeys, size_t roundKeyIndex) const
{
    auto _state    = reinterpret_cast<uint64_t*>(state);
    auto _roundKey = reinterpret_cast<const uint64_t*>(roundKeys + roundKeyIndex * s_kBlockSize);
    for (size_t i = 0; i < s_kBlockSize / sizeof(uint64_t); ++i)
    {
        _state[i] ^= _roundKey[i];
    }
}

void AesCipher::work(std::istream& in, std::mutex& readMutex, std::ostream& out, std::mutex& writeMutex,
                     CipherDirection direction, size_t rounds, const uint8_t* roundKeys, uint64_t pos,
                     uint64_t count) const
{
    auto inBuffer  = new uint8_t[s_kBufferSize];
    auto outBuffer = new uint8_t[s_kBufferSize];

    for (uint64_t right = pos + count; pos < right; pos += s_kBufferSize)
    {
        uint64_t lastRead;

        {
            std::lock_guard<std::mutex> lock(readMutex);
            in.seekg(safe_integral_cast<std::__1::streamoff>(pos));
            in.read(reinterpret_cast<char*>(inBuffer),
                    safe_integral_cast<std::__1::streamsize>(std::min(s_kBufferSize, count)));
            lastRead = safe_integral_cast<uint64_t>(in.gcount());
        }

        // Padding with zero
        if (lastRead % s_kBlockSize != 0)
        {
            std::fill_n(inBuffer + lastRead, (lastRead / s_kBlockSize + 1) * s_kBlockSize - lastRead, 0);
            lastRead = (lastRead / s_kBlockSize + 1) * s_kBlockSize;
        }

        for (size_t i = 0; i < lastRead; i += s_kBlockSize)
        {
            if (direction == CipherDirection::FORWARD)
            {
                encryptBlock(inBuffer + i, rounds, roundKeys, outBuffer + i);
            }
            else if (direction == CipherDirection::INVERSE)
            {
                decryptBlock(inBuffer + i, rounds, roundKeys, outBuffer + i);
            }
            else
            {
                assert(false);
            }
        }

        {
            std::lock_guard<std::mutex> lock(writeMutex);
            out.seekp(safe_integral_cast<int64_t>(pos));
            out.write(reinterpret_cast<char*>(outBuffer), safe_integral_cast<std::__1::streamsize>(lastRead));
        }

        count -= lastRead;
    }

    delete[] inBuffer;
    delete[] outBuffer;
}

void AesCipher::encryptBlock(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output) const
{
    uint8_t state[s_kBlockSize];
    std::copy_n(input, s_kBlockSize, state);

    // AddRoundKey
    addRoundKey(state, roundKeys, 0);

    for (size_t round = 1; round < rounds; ++round)
    {
        // SubBytes + ShiftRows + MixColumns
        uint8_t nextState[s_kBlockSize];

        nextState[0]  = kXtimes2Sbox[state[0]] ^ kXtimes3Sbox[state[5]] ^ kSbox[state[10]] ^ kSbox[state[15]];
        nextState[1]  = kSbox[state[0]] ^ kXtimes2Sbox[state[5]] ^ kXtimes3Sbox[state[10]] ^ kSbox[state[15]];
        nextState[2]  = kSbox[state[0]] ^ kSbox[state[5]] ^ kXtimes2Sbox[state[10]] ^ kXtimes3Sbox[state[15]];
        nextState[3]  = kXtimes3Sbox[state[0]] ^ kSbox[state[5]] ^ kSbox[state[10]] ^ kXtimes2Sbox[state[15]];
        nextState[4]  = kXtimes2Sbox[state[4]] ^ kXtimes3Sbox[state[9]] ^ kSbox[state[14]] ^ kSbox[state[3]];
        nextState[5]  = kSbox[state[4]] ^ kXtimes2Sbox[state[9]] ^ kXtimes3Sbox[state[14]] ^ kSbox[state[3]];
        nextState[6]  = kSbox[state[4]] ^ kSbox[state[9]] ^ kXtimes2Sbox[state[14]] ^ kXtimes3Sbox[state[3]];
        nextState[7]  = kXtimes3Sbox[state[4]] ^ kSbox[state[9]] ^ kSbox[state[14]] ^ kXtimes2Sbox[state[3]];
        nextState[8]  = kXtimes2Sbox[state[8]] ^ kXtimes3Sbox[state[13]] ^ kSbox[state[2]] ^ kSbox[state[7]];
        nextState[9]  = kSbox[state[8]] ^ kXtimes2Sbox[state[13]] ^ kXtimes3Sbox[state[2]] ^ kSbox[state[7]];
        nextState[10] = kSbox[state[8]] ^ kSbox[state[13]] ^ kXtimes2Sbox[state[2]] ^ kXtimes3Sbox[state[7]];
        nextState[11] = kXtimes3Sbox[state[8]] ^ kSbox[state[13]] ^ kSbox[state[2]] ^ kXtimes2Sbox[state[7]];
        nextState[12] = kXtimes2Sbox[state[12]] ^ kXtimes3Sbox[state[1]] ^ kSbox[state[6]] ^ kSbox[state[11]];
        nextState[13] = kSbox[state[12]] ^ kXtimes2Sbox[state[1]] ^ kXtimes3Sbox[state[6]] ^ kSbox[state[11]];
        nextState[14] = kSbox[state[12]] ^ kSbox[state[1]] ^ kXtimes2Sbox[state[6]] ^ kXtimes3Sbox[state[11]];
        nextState[15] = kXtimes3Sbox[state[12]] ^ kSbox[state[1]] ^ kSbox[state[6]] ^ kXtimes2Sbox[state[11]];

        std::copy_n(nextState, s_kBlockSize, state);

        // AddRoundKey
        addRoundKey(state, roundKeys, round);
    }

    {
        // SubBytes + ShiftRows
        uint8_t tmp;

        // Row 0 (Substitute)
        state[0]  = kSbox[state[0]];
        state[4]  = kSbox[state[4]];
        state[8]  = kSbox[state[8]];
        state[12] = kSbox[state[12]];

        // Row 1 (Substitute + Rotate)
        tmp       = kSbox[state[1]];
        state[1]  = kSbox[state[5]];
        state[5]  = kSbox[state[9]];
        state[9]  = kSbox[state[13]];
        state[13] = tmp;

        // Row 2 (Substitute + Rotate)
        tmp       = kSbox[state[2]];
        state[2]  = kSbox[state[10]];
        state[10] = tmp;
        tmp       = kSbox[state[6]];
        state[6]  = kSbox[state[14]];
        state[14] = tmp;

        // Row 3 (Substitute + Rotate)
        tmp       = kSbox[state[15]];
        state[15] = kSbox[state[11]];
        state[11] = kSbox[state[7]];
        state[7]  = kSbox[state[3]];
        state[3]  = tmp;
    }

    // AddRoundKey
    addRoundKey(state, roundKeys, rounds);

    std::copy_n(state, s_kBlockSize, output);
}

void AesCipher::decryptBlock(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output) const
{
    uint8_t state[s_kBlockSize];
    std::copy_n(input, s_kBlockSize, state);

    size_t round = rounds;

    // AddRoundKey
    addRoundKey(state, roundKeys, round);

    // InvShiftRows + InvSubBytes
    {
        uint8_t tmp;

        // Row 0
        state[0]  = kInvSbox[state[0]];
        state[4]  = kInvSbox[state[4]];
        state[8]  = kInvSbox[state[8]];
        state[12] = kInvSbox[state[12]];

        // Row 1
        tmp       = kInvSbox[state[13]];
        state[13] = kInvSbox[state[9]];
        state[9]  = kInvSbox[state[5]];
        state[5]  = kInvSbox[state[1]];
        state[1]  = tmp;

        // Row 2
        tmp       = kInvSbox[state[2]];
        state[2]  = kInvSbox[state[10]];
        state[10] = tmp;
        tmp       = kInvSbox[state[6]];
        state[6]  = kInvSbox[state[14]];
        state[14] = tmp;

        // Row 3
        tmp       = kInvSbox[state[3]];
        state[3]  = kInvSbox[state[7]];
        state[7]  = kInvSbox[state[11]];
        state[11] = kInvSbox[state[15]];
        state[15] = tmp;
    }

    while (round--)
    {
        // AddRoundKey
        addRoundKey(state, roundKeys, round);

        if (round != 0)
        {
            // InvMixColumns + InvShiftRows + InvSubBytes
            uint8_t nextState[s_kBlockSize];

            // Column 0
            nextState[0]  = kXtimesE[state[0]] ^ kXtimesB[state[1]] ^ kXtimesD[state[2]] ^ kXtimes9[state[3]];
            nextState[5]  = kXtimes9[state[0]] ^ kXtimesE[state[1]] ^ kXtimesB[state[2]] ^ kXtimesD[state[3]];
            nextState[10] = kXtimesD[state[0]] ^ kXtimes9[state[1]] ^ kXtimesE[state[2]] ^ kXtimesB[state[3]];
            nextState[15] = kXtimesB[state[0]] ^ kXtimesD[state[1]] ^ kXtimes9[state[2]] ^ kXtimesE[state[3]];

            // Column 1
            nextState[4]  = kXtimesE[state[4]] ^ kXtimesB[state[5]] ^ kXtimesD[state[6]] ^ kXtimes9[state[7]];
            nextState[9]  = kXtimes9[state[4]] ^ kXtimesE[state[5]] ^ kXtimesB[state[6]] ^ kXtimesD[state[7]];
            nextState[14] = kXtimesD[state[4]] ^ kXtimes9[state[5]] ^ kXtimesE[state[6]] ^ kXtimesB[state[7]];
            nextState[3]  = kXtimesB[state[4]] ^ kXtimesD[state[5]] ^ kXtimes9[state[6]] ^ kXtimesE[state[7]];

            // Column 2
            nextState[8]  = kXtimesE[state[8]] ^ kXtimesB[state[9]] ^ kXtimesD[state[10]] ^ kXtimes9[state[11]];
            nextState[13] = kXtimes9[state[8]] ^ kXtimesE[state[9]] ^ kXtimesB[state[10]] ^ kXtimesD[state[11]];
            nextState[2]  = kXtimesD[state[8]] ^ kXtimes9[state[9]] ^ kXtimesE[state[10]] ^ kXtimesB[state[11]];
            nextState[7]  = kXtimesB[state[8]] ^ kXtimesD[state[9]] ^ kXtimes9[state[10]] ^ kXtimesE[state[11]];

            // Column 3
            nextState[12] = kXtimesE[state[12]] ^ kXtimesB[state[13]] ^ kXtimesD[state[14]] ^ kXtimes9[state[15]];
            nextState[1]  = kXtimes9[state[12]] ^ kXtimesE[state[13]] ^ kXtimesB[state[14]] ^ kXtimesD[state[15]];
            nextState[6]  = kXtimesD[state[12]] ^ kXtimes9[state[13]] ^ kXtimesE[state[14]] ^ kXtimesB[state[15]];
            nextState[11] = kXtimesB[state[12]] ^ kXtimesD[state[13]] ^ kXtimes9[state[14]] ^ kXtimesE[state[15]];

            for (size_t i = 0; i < s_kBlockSize; i++)
                state[i] = kInvSbox[nextState[i]];
        }
    }

    std::copy_n(state, s_kBlockSize, output);
}
