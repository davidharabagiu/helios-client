#ifndef CIPHER_H
#define CIPHER_H

#include <cstdint>
#include <cstddef>

/**
 * @class Cipher
 * @brief Cipher interface for cryptographic algorithms
 */
class Cipher
{
public:
    /**
     * @brief Destructor
     */
    virtual ~Cipher() = default;

    /**
     * @brief Set the encryption / decryption key and do the initial preparations for the operations that will follow.
     * @param key - Key
     */
    virtual void setKey(const uint8_t* key) = 0;

    /**
     * @brief Run the encryption cipher algorithm
     * @param in - Input (plain text) buffer
     * @param count - Number of bytes in the input buffer
     * @param out - Output (encrypted) buffer
     * @return Number of encrypted data bytes
     */
    virtual uint64_t encrypt(const uint8_t* in, uint64_t count, uint8_t* out) = 0;

    /**
     * @brief Run the decryption cipher algorithm
     * @param in - Input (encrypted) buffer
     * @param count - Number of bytes in the input buffer
     * @param out - Output (plain text) buffer
     */
    virtual void decrypt(const uint8_t* in, uint64_t count, uint8_t* out) = 0;

    /**
     * @brief Returns the block size of an encryption in bytes. The output size will be a multiple of this value.
     * @return uint16_t
     */
    virtual size_t blockSize() const = 0;
};

#endif  // CIPHER_H
