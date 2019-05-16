#ifndef AES_COMMONDEFS_H
#define AES_COMMONDEFS_H

namespace Aes
{
/**
 * @brief Available key sizes for the AES algorithm
 */
enum class KeySize
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
 * @brief Cipher direction
 */
enum class CipherDirection
{
    /**
     * @brief Encryption
     */
    FORWARD,

    /**
     * @brief Decryption
     */
    INVERSE
};
}  // namespace Aes

#endif  // AES_COMMONDEFS_H
