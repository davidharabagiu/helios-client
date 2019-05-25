#ifndef RSA_H
#define RSA_H

#include <string>
#include <vector>
#include <cstdint>

/**
 * @class Rsa
 * @brief Utility class which provides key generation, data encryption and data decyrption using the RSA public-key
 * cryptosystem
 */
class Rsa
{
public:
    /**
     * @brief Destructor
     */
    virtual ~Rsa() = default;

    /**
     * @brief Generate a private-public key pair and store them in separate files (base64 format)
     * @param privateKeyFile - Path of the file where to store the private key
     * @param publicKeyFile - Path of the file where to store the public key
     */
    virtual void generateKeyPair(const std::string& privateKeyFile, const std::string& publicKeyFile) const = 0;

    /**
     * @brief Perform an encryption using a public key
     * @param publicKeyFile - Path of the file where the public key is stored
     * @param message - Message to encrypt
     * @return Encrypted message
     */
    virtual std::vector<uint8_t> encrypt(const std::string&          publicKeyFile,
                                         const std::vector<uint8_t>& message) const = 0;

    /**
     * @brief Perform a decryption using a private key
     * @param privateKeyFile - Path of the file where the private key is stored
     * @param message - Message to decrypt
     * @return Decrypted message
     */
    virtual std::vector<uint8_t> decrypt(const std::string&          privateKeyFile,
                                         const std::vector<uint8_t>& message) const = 0;
};

#endif  // RSA_H
