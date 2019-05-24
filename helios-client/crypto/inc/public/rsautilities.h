#ifndef RSAUTILITIES_H
#define RSAUTILITIES_H

#include <string>
#include <cstdint>

/**
 * @class RsaUtilities
 * @brief Utility class which provides key generation, data encryption and data decyrption using the RSA public-key
 * cryptosystem
 */
class RsaUtilities
{
public:
    /**
     * @brief Destructor
     */
    virtual ~RsaUtilities() = default;

    /**
     * @brief Generate a private-public key pair and store them in separate files (base64 format)
     * @param length - Length of keys in bytes
     * @param privateKeyFile - Path of the file where to store the private key
     * @param publicKeyFile - Path of the file where to store the public key
     */
    virtual void generateKeyPairFiles(uint16_t length, const std::string& privateKeyFile,
                                      const std::string& publicKeyFile) const = 0;

    /**
     * @brief Perform an encryption using a public key
     * @param publicKeyFile - Path of the file where the public key is stored
     * @param in - Message to encrypt
     * @param out - Encrypted message output
     */
    virtual void encrypt(const std::string& publicKeyFile, const uint8_t* in, uint8_t* out) const = 0;

    /**
     * @brief Perform a decryption using a private key
     * @param privateKeyFile - Path of the file where the private key is stored
     * @param in - Message to decrypt
     * @param out - Decrypted message output
     */
    virtual void decrypt(const std::string& privateKeyFile, const uint8_t* in, uint8_t* out) const = 0;
};

#endif  // RSAUTILITIES_H
