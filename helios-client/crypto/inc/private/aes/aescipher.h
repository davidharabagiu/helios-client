#ifndef AES_AESCIPHER_H
#define AES_AESCIPHER_H

#include <vector>
#include <istream>
#include <ostream>
#include <functional>
#include <cstdint>
#include <mutex>

#include "aes/commondefs.h"
#include "executor.h"

namespace Aes
{
/**
 * @class AesCipher
 * @brief Implementation of the AES cipher
 */
class AesCipher
{
public:
    /**
     * @brief Constructor
     * @param numThreads - Number of executor threads
     */
    AesCipher(int numThreads);

    /**
     * @brief Run the AES encryption or decryption cipher
     * @param key - Key, its length depends on variant
     * @param in - Input stream
     * @param out - Output stream
     */
    void run(const uint8_t* key, std::istream& in, std::ostream& out, KeySize keySize, CipherDirection direction);

private:
    /**
     * @brief Generate keys for each round
     * @param key - Input key
     * @param keySize - Key size in bytes
     * @param rounds - Number of rounds
     * @param roundKeys - Output round keys
     */
    void generateRoundKeys(const uint8_t* key, size_t keySize, size_t rounds, uint8_t* roundKeys) const;

    /**
     * @brief Add (bitwise XOR) the round key into the state
     * @param state - State
     * @param roundKeys - Round keys
     * @param roundKeyIndex - Index of the round key in roundKeys
     */
    void addRoundKey(uint8_t* state, const uint8_t* roundKeys, size_t roundKeyIndex) const;

    /**
     * @brief Perform an encryption or decryption on a given sequence of bytes
     * @param in - Input stream
     * @param readMutex - Input stream mutex
     * @param out - Output stream
     * @param writeMutex - Output write mutex
     * @param direction - Cipher direction
     * @param rounds - Number of rounds
     * @param roundKeys - Round keys
     * @param pos - Starting position of the sequence
     * @param count - Number of bytes in the sequence
     */
    void work(std::istream& in, std::mutex& readMutex, std::ostream& out, std::mutex& writeMutex,
              CipherDirection direction, size_t rounds, const uint8_t* roundKeys, uint64_t pos, uint64_t count) const;

    /**
     * @brief Perform AES encryption of a block of data
     * @param input - Input block of data (kBlockSize bytes)
     * @param rounds - Number of rounds
     * @param roundKeys - Round keys
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here.
     */
    void encryptBlock(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output) const;

    /**
     * @brief - Perform AES decryption of a block of data
     * @param input - Input block of data (kBlockSize bytes)
     * @param roundKeys - Round keys
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here.
     */
    void decryptBlock(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output) const;

private:
    /**
     * @brief Executors
     */
    std::vector<std::unique_ptr<Executor>> m_executors;

    /**
     * @brief Number of bytes in a data block or in a subkey
     */
    static const size_t s_kBlockSize;

    /**
     * @brief Buffer size in bytes for buffered stream read operations
     */
    static const uint64_t s_kBufferSize;
};
}  // namespace Aes

#endif  // AES_AESCIPHER_H
