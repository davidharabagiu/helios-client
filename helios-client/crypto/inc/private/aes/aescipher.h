#ifndef AES_AESCIPHER_H
#define AES_AESCIPHER_H

#include <vector>
#include <istream>
#include <ostream>
#include <functional>
#include <cstdint>

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
     * @param variant - AES cipher variant
     * @param numThreads - Number of threads
     */
    AesCipher(AesVariant variant, int numThreads);

    /**
     * @brief Run the AES encryption cipher asynchronously
     * @param key - Key, its length depends on variant
     * @param in - Input stream
     * @param out - Output stream
     * @param callback - Callback
     */
    void encryptAsync(const uint8_t* key, std::istream& in, std::ostream& out, std::function<void()> callback);

    /**
     * @brief Run the AES decryption cipher asynchronously
     * @param key - Key, its length depends on variant
     * @param in - Input stream
     * @param out - Output stream
     * @param callback - Callback
     */
    void decryptAsync(const uint8_t* key, std::istream& in, std::ostream& out, std::function<void()> callback);

private:
    /**
     * @brief Generate keys for each round
     * @param key - Input key
     * @param roundKeys - Output round keys
     */
    void generateRoundKeys(const uint8_t* key, uint8_t* roundKeys) const;

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
};
}  // namespace Aes

#endif  // AES_AESCIPHER_H
