#ifndef AES128DEFS_H
#define AES128DEFS_H

#include <cstddef>

/**
 * @namespace AesDefs
 * @brief Common definitions for AES-128 Implementation
 */
namespace Aes128Defs
{
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
 */
const size_t kKeySize = 16;

/**
 * @brief Number of columns in a key
 */
const size_t kKeyColumnSize = 4;

/**
 * @brief Number of rounds
 */
const size_t kNumberOfRounds = 10;
}  // namespace Aes128Defs

#endif  // AES128DEFS_H
