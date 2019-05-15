#include <algorithm>
#include <memory>
#include <cassert>

#include "aes/aescipher.h"
#include "aes/commondefs.h"
#include "aes/tables.h"
#include "aes/encryptionwork.h"
#include "aes/decryptionwork.h"
#include "typeconversions.h"
#include "autoresetevent.h"

using namespace Aes;

const uint64_t AesCipher::s_kBufferSize = 16 * 1024 * 1024;  // 16 MB

AesCipher::AesCipher(AesVariant variant, int numThreads)
    : m_kRounds(numberOfRounds(variant))
    , m_kKeySize(keySize(variant))
    , m_executors(safe_integral_cast<size_t>(numThreads))
{
    static_assert(s_kBufferSize % kBlockSize == 0);

    for (auto& executor : m_executors)
    {
        executor.reset(new Executor());
    }
}

void AesCipher::run(const uint8_t* key, std::istream& in, std::ostream& out, CipherDirection direction)
{
    // Generate round keys
    auto roundKeys = new uint8_t[kBlockSize * (m_kRounds + 1)];
    generateRoundKeys(key, roundKeys);

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

    // We need to divide the work in multiples of kBlockSize bytes
    if (count % kBlockSize != 0)
    {
        count = (count / kBlockSize + 1) * kBlockSize;
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
            [this, &in, &out, &events, &readMutex, &writeMutex, roundKeys, direction](size_t idx, uint64_t pos,
                                                                                      uint64_t count) {
                work(in, readMutex, out, writeMutex, direction, roundKeys, pos, count);
                events[idx]->set();
            },
            executorIdx, pos, count);

        pos += count;
    }

    // Wait for completion
    for (size_t executorIdx = 0; executorIdx != m_executors.size(); ++executorIdx)
    {
        events[executorIdx]->wait();
    }

    delete[] roundKeys;
}

void AesCipher::generateRoundKeys(const uint8_t* key, uint8_t* roundKeys) const
{
    auto*        _key       = reinterpret_cast<const uint32_t*>(key);
    auto*        _roundKeys = reinterpret_cast<uint32_t*>(roundKeys);
    const size_t _keySize   = m_kKeySize / sizeof(uint32_t);
    uint32_t     tmp;

    std::copy_n(_key, _keySize, _roundKeys);

    for (size_t i = _keySize; i != 4 * (m_kRounds + 1); ++i)
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

void AesCipher::work(std::istream& in, std::mutex& readMutex, std::ostream& out, std::mutex& writeMutex,
                     CipherDirection direction, const uint8_t* roundKeys, uint64_t pos, uint64_t count) const
{
    auto inBuffer  = new uint8_t[s_kBufferSize];
    auto outBuffer = new uint8_t[s_kBufferSize];

    for (uint64_t right = pos + count; pos < right; pos += s_kBufferSize)
    {
        uint64_t lastRead;

        {
            std::lock_guard<std::mutex> lock(readMutex);
            in.seekg(safe_integral_cast<std::__1::streamoff>(pos));
            in.read(reinterpret_cast<char*>(inBuffer), s_kBufferSize);
            lastRead = safe_integral_cast<uint64_t>(in.gcount());
        }

        // Padding with zero
        if (lastRead % kBlockSize != 0)
        {
            std::fill_n(inBuffer + pos + lastRead, (lastRead / kBlockSize + 1) * kBlockSize, 0);
            lastRead = (lastRead / kBlockSize + 1) * kBlockSize;
        }

        for (size_t i = 0; i < lastRead; i += kBlockSize)
        {
            if (direction == CipherDirection::FORWARD)
            {
                EncryptionWork(inBuffer + i, m_kRounds, roundKeys, outBuffer + i)();
            }
            else if (direction == CipherDirection::INVERSE)
            {
                DecryptionWork(inBuffer + i, m_kRounds, roundKeys, outBuffer + i)();
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
    }

    delete[] inBuffer;
    delete[] outBuffer;
}
