#ifndef CIPHER_H
#define CIPHER_H

#include <cstdint>
#include <istream>
#include <ostream>

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
     * @brief Run the encryption cipher algorithm
     * @param key - Encryption key
     * @param in - Input (plain text) stream
     * @param out - Output (encrypted) stream
     */
    virtual void encrypt(const uint8_t* key, std::istream& in, std::ostream& out) = 0;

    /**
     * @brief Run the decryption cipher algorithm
     * @param key - Decryption key
     * @param in - Input (encrypted) stream
     * @param out - Output (plain text) stream
     */
    virtual void decrypt(const uint8_t* key, std::istream& in, std::ostream& out) = 0;
};

#endif  // CIPHER_H
