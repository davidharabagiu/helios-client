#include <algorithm>
#include <memory>
#include <cassert>

#include "aes/aescipher.h"
#include "aes/tables.h"
#include "aes/utils.h"
#include "typeconversions.h"
#include "autoresetevent.h"

using namespace Aes;

const size_t AesCipher::s_kBlockSize = 16;

AesCipher::AesCipher(KeySize keySize, int numThreads)
    : m_kKeySize(keySize)
    , m_roundKeys(new uint8_t[s_kBlockSize * (numberOfRounds(keySize) + 1)])
    , m_executors(safe_integral_cast<size_t>(numThreads))
{
    for (auto& executor : m_executors)
    {
        executor.reset(new Executor());
    }
}

AesCipher::~AesCipher()
{
    delete[] m_roundKeys;
}

void AesCipher::setKey(const uint8_t* key)
{
    generateRoundKeys(key);
}

uint64_t AesCipher::encrypt(const uint8_t* in, uint64_t count, uint8_t* out)
{
    return run(in, count, out, CipherDirection::FORWARD);
}

void AesCipher::decrypt(const uint8_t* in, uint64_t count, uint8_t* out)
{
    assert(count % s_kBlockSize == 0);
    run(in, count, out, CipherDirection::INVERSE);
}

uint64_t AesCipher::run(const uint8_t* in, uint64_t count, uint8_t* out, CipherDirection direction)
{
    // Events for waiting on executors
    std::vector<std::shared_ptr<AutoResetEvent>> events;

    uint64_t pos          = 0;
    uint64_t countPerWork = count / m_executors.size();
    size_t   rounds       = numberOfRounds(m_kKeySize);

    // We need to divide the work in multiples of s_kBlockSize bytes
    if (countPerWork % s_kBlockSize != 0 || countPerWork == 0)
    {
        countPerWork = (countPerWork / s_kBlockSize + 1) * s_kBlockSize;
    }

    // Create works
    for (size_t executorIdx = 0; executorIdx != m_executors.size(); ++executorIdx)
    {
        // If we reach the end of the buffer, there is no need to create more works
        if (pos >= count)
        {
            break;
        }

        events.push_back(std::make_shared<AutoResetEvent>());

        m_executors[executorIdx]->post(
            [this, &events](size_t idx, const uint8_t* in, uint64_t count, uint8_t* out, size_t rounds,
                            CipherDirection direction) {
                work(in, count, out, rounds, direction);
                events[idx]->set();
            },
            executorIdx, in + pos, std::min(countPerWork, count - pos), out + pos, rounds, direction);

        pos += countPerWork;
    }

    // Wait for completion
    for (size_t executorIdx = 0; executorIdx != events.size(); ++executorIdx)
    {
        events[executorIdx]->wait();
    }

    if (count % s_kBlockSize != 0)
    {
        return (count / s_kBlockSize + 1) * s_kBlockSize;
    }
    return count;
}

void AesCipher::generateRoundKeys(const uint8_t* key)
{
    auto*        _key       = reinterpret_cast<const uint32_t*>(key);
    auto*        _roundKeys = reinterpret_cast<uint32_t*>(m_roundKeys);
    const size_t _keySize   = bytesInKey(m_kKeySize) / sizeof(uint32_t);
    size_t       rounds     = numberOfRounds(m_kKeySize);
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

void AesCipher::addRoundKey(uint8_t* state, size_t roundKeyIndex) const
{
    auto _state    = reinterpret_cast<uint64_t*>(state);
    auto _roundKey = reinterpret_cast<const uint64_t*>(m_roundKeys + roundKeyIndex * s_kBlockSize);
    for (size_t i = 0; i < s_kBlockSize / sizeof(uint64_t); ++i)
    {
        _state[i] ^= _roundKey[i];
    }
}

void AesCipher::work(const uint8_t* in, uint64_t count, uint8_t* out, size_t rounds, CipherDirection direction) const
{
    auto blockSize = safe_integral_cast<uint64_t>(s_kBlockSize);

    if (direction == CipherDirection::FORWARD)
    {
        for (; count >= blockSize; count -= blockSize, in += blockSize, out += blockSize)
        {
            encryptBlock(in, rounds, out);
        }

        // Padding with zero if there are bytes left
        if (count > 0)
        {
            uint8_t temp[s_kBlockSize];
            std::copy_n(in, count, temp);
            std::fill_n(temp + count, s_kBlockSize - count, 0);
            encryptBlock(temp, rounds, out);
        }
    }
    else if (direction == CipherDirection::INVERSE)
    {
        for (; count > 0; count -= std::min(count, blockSize), in += blockSize, out += blockSize)
        {
            decryptBlock(in, rounds, out);
        }
    }
    else
    {
        assert(false);
    }
}

void AesCipher::encryptBlock(const uint8_t* input, size_t rounds, uint8_t* output) const
{
    uint8_t state[s_kBlockSize];
    std::copy_n(input, s_kBlockSize, state);

    // AddRoundKey
    addRoundKey(state, 0);

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
        addRoundKey(state, round);
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
    addRoundKey(state, rounds);

    std::copy_n(state, s_kBlockSize, output);
}

void AesCipher::decryptBlock(const uint8_t* input, size_t rounds, uint8_t* output) const
{
    uint8_t state[s_kBlockSize];
    std::copy_n(input, s_kBlockSize, state);

    size_t round = rounds;

    // AddRoundKey
    addRoundKey(state, round);

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
        addRoundKey(state, round);

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
