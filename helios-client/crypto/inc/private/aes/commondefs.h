#ifndef AES_COMMONDEFS_H
#define AES_COMMONDEFS_H

#include <cstddef>
#include <cassert>

/**
 * @namespace AesDefs
 * @brief Common definitions for AES-128 Implementation
 */
namespace Aes
{
/**
 * @brief Variants of the AES algorithm
 */
enum class AesVariant
{
    /**
     * @brief AES with 128-bit keys
     */
    AES128,

    /**
     * @brief AES with 192-bit keys
     */
    AES192,

    /**
     * @brief AES with 256-bit keys
     */
    AES256
};

/**
 * @brief Number of bytes in a data block or in a subkey
 */
const size_t kBlockSize = 16;

/**
 * @brief Number of columns in a data block or in a subkey
 */
const size_t kBlockColumns = 4;

/**
 * @brief Number of bytes in a key
 * @param variant - AES variant
 * @return size_t
 */
inline size_t keySize(const AesVariant variant)
{
    if (variant == AesVariant::AES128)
    {
        return 16;
    }
    else if (variant == AesVariant::AES192)
    {
        return 24;
    }
    else if (variant == AesVariant::AES256)
    {
        return 32;
    }
    assert(false);
}

/**
 * @brief Number of rounds
 * @param variant - AES variant
 * @return int
 */
inline int numberOfRounds(const AesVariant variant)
{
    if (variant == AesVariant::AES128)
    {
        return 10;
    }
    else if (variant == AesVariant::AES192)
    {
        return 12;
    }
    else if (variant == AesVariant::AES256)
    {
        return 14;
    }
    assert(false);
}

/**
 * @brief Number of columns in a 128 bit key
 */
const size_t kKeyColumnSize128 = 4;
}  // namespace Aes

#endif  // AES_COMMONDEFS_H
