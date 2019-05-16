#ifndef AES_AESCIPHER_H
#define AES_AESCIPHER_H

#include <vector>
#include <functional>

#include "cipher.h"
#include "aes/commondefs.h"
#include "executor.h"

namespace Aes
{
/**
 * @class AesCipher
 * @brief Implementation of the AES cipher
 */
class AesCipher : public Cipher
{
public:
    /**
     * @brief Constructor
     * @param keySize - Key size
     * @param numThreads - Number of executor threads
     */
    AesCipher(KeySize keySize, int numThreads);

    /**
     * @brief Destructor
     */
    ~AesCipher();

public:  // from Cipher
    void setKey(const uint8_t* key) override;
    void encrypt(const uint8_t* in, uint64_t count, uint8_t* out) override;
    void decrypt(const uint8_t* in, uint64_t count, uint8_t* out) override;

public:
    /**
     * @brief Run the AES encryption or decryption cipher
     * @param in - Input buffer
     * @param count - Number of bytes in input buffer
     * @param out - Output buffer
     * @param direction - Encryption / decryption
     */
    void run(const uint8_t* in, uint64_t count, uint8_t* out, CipherDirection direction);

private:
    /**
     * @brief Generate keys for each round
     * @param key - Input key
     */
    void generateRoundKeys(const uint8_t* key);

    /**
     * @brief Add (bitwise XOR) the round key into the state
     * @param state - State
     * @param roundKeyIndex - Index of the round key in roundKeys
     */
    void addRoundKey(uint8_t* state, size_t roundKeyIndex) const;

    /**
     * @brief Perform an encryption or decryption on a given sequence of bytes
     * @param in - Input buffer
     * @param count - Number of bytes in the input buffer
     * @param out - Output buffer
     * @param rounds - Number of rounds
     * @param direction - Cipher direction
     */
    void work(const uint8_t* in, uint64_t count, uint8_t* out, size_t rounds, CipherDirection direction) const;

    /**
     * @brief Perform AES encryption of a block of data
     * @param input - Input block of data (kBlockSize bytes)
     * @param rounds - Number of rounds
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here.
     */
    void encryptBlock(const uint8_t* input, size_t rounds, uint8_t* output) const;

    /**
     * @brief Perform AES decryption of a block of data
     * @param input - Input block of data (kBlockSize bytes)
     * @param rounds - Number of rounds
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here.
     */
    void decryptBlock(const uint8_t* input, size_t rounds, uint8_t* output) const;

private:
    /**
     * @brief Key size
     */
    const KeySize m_kKeySize;

    /**
     * @brief Round keys
     */
    uint8_t* m_roundKeys;

    /**
     * @brief Executors
     */
    std::vector<std::unique_ptr<Executor>> m_executors;

    /**
     * @brief Number of bytes in a data block or in a subkey
     */
    static const size_t s_kBlockSize;
};
}  // namespace Aes

#endif  // AES_AESCIPHER_H
