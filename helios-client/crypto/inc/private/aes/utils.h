#ifndef AES_UTILS_H
#define AES_UTILS_H

#include <cassert>
#include <cstddef>

#include "commondefs.h"

namespace Aes
{
/**
 * @brief Number of bytes in a key
 * @param keySize - Key size enum
 * @return size_t
 */
inline size_t bytesInKey(const KeySize keySize)
{
    if (keySize == KeySize::AES128)
    {
        return 16;
    }
    else if (keySize == KeySize::AES192)
    {
        return 24;
    }
    else if (keySize == KeySize::AES256)
    {
        return 32;
    }
    assert(false);
}

/**
 * @brief Number of rounds (not including the initial round)
 * @param keySize - Key size
 * @return int
 */
inline size_t numberOfRounds(const KeySize keySize)
{
    if (keySize == KeySize::AES128)
    {
        return 10;
    }
    else if (keySize == KeySize::AES192)
    {
        return 12;
    }
    else if (keySize == KeySize::AES256)
    {
        return 14;
    }
    assert(false);
}
}  // namespace Aes

#endif  // AES_UTILS_H
