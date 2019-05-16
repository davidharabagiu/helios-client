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
     * @brief Cipher direction
     */
    enum class CipherDirection
    {
        FORWARD,
        INVERSE
    };

public:
    /**
     * @brief Constructor
     * @param variant - AES cipher variant
     * @param numThreads - Number of threads
     */
    AesCipher(AesVariant variant, int numThreads);

    /**
     * @brief Run the AES encryption or decryption cipher
     * @param key - Key, its length depends on variant
     * @param in - Input stream
     * @param out - Output stream
     */
    void run(const uint8_t* key, std::istream& in, std::ostream& out, CipherDirection direction);

private:
    /**
     * @brief Generate keys for each round
     * @param key - Input key
     * @param roundKeys - Output round keys
     */
    void generateRoundKeys(const uint8_t* key, uint8_t* roundKeys) const;

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
     * @param roundKeys - Round keys
     * @param pos - Starting position of the sequence
     * @param count - Number of bytes in the sequence
     */
    void work(std::istream& in, std::mutex& readMutex, std::ostream& out, std::mutex& writeMutex,
              CipherDirection direction, const uint8_t* roundKeys, uint64_t pos, uint64_t count) const;

    /**
     * @param input - Input block of data (kBlockSize bytes)
     * @param roundKeys - Round keys
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here.
     */
    void encryptBlock(const uint8_t* input, const uint8_t* roundKeys, uint8_t* output) const;

    /**
     * @param input - Input block of data (kBlockSize bytes)
     * @param roundKeys - Round keys
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here.
     */
    void decryptBlock(const uint8_t* input, const uint8_t* roundKeys, uint8_t* output) const;

private:
    /**
     * @brief Number of rounds
     */
    const size_t m_kRounds;

    /**
     * @brief Key size in bytes
     */
    const size_t m_kKeySize;

    /**
     * @brief Executors
     */
    std::vector<std::unique_ptr<Executor>> m_executors;

    /**
     * @brief Buffer size in bytes for buffered stream read operations
     */
    static const uint64_t s_kBufferSize;
};
}  // namespace Aes

#endif  // AES_AESCIPHER_H
