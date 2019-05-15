#include <algorithm>

#include "aes/aescipher.h"
#include "aes/commondefs.h"
#include "aes/tables.h"
#include "typeconversions.h"

using namespace Aes;

AesCipher::AesCipher(AesVariant variant, int numThreads)
    : m_kRounds(numberOfRounds(variant))
    , m_kKeySize(keySize(variant))
    , m_executors(safe_integral_cast<size_t>(numThreads))
{
    for (auto& executor : m_executors)
    {
        executor.reset(new Executor());
    }
}

void AesCipher::generateRoundKeys(const uint8_t* key, uint8_t* roundKeys) const
{
    auto*        _key       = reinterpret_cast<const uint32_t*>(key);
    auto*        _roundKeys = reinterpret_cast<uint32_t*>(roundKeys);
    const size_t _keySize   = m_kKeySize / sizeof(uint32_t);
    uint32_t     tmp;

    std::copy_n(_key, _keySize, _roundKeys);

    for (size_t i = _keySize; i < 4 * (m_kRounds + 1); ++i)
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
