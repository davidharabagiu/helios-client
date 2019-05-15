#ifndef AES_AES_H
#define AES_AES_H

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
 * @class Aes
 * @brief Implementation of the AES cypher
 */
class Aes
{
public:
    /**
     * @brief Constructor
     * @param variant - AES cypher variant
     * @param numThreads - Number of threads
     */
    Aes(AesVariant variant, int numThreads);

    /**
     * @brief Run the AES encryption cypher asynchronously
     * @param key - Key, its length depends on variant
     * @param in - Input stream
     * @param out - Output stream
     * @param callback - Callback
     */
    void encryptAsync(const uint8_t* key, std::istream& in, std::ostream& out, std::function<void()> callback);

    /**
     * @brief Run the AES decryption cypher asynchronously
     * @param key - Key, its length depends on variant
     * @param in - Input stream
     * @param out - Output stream
     * @param callback - Callback
     */
    void decryptAsync(const uint8_t* key, std::istream& in, std::ostream& out, std::function<void()> callback);

private:
    /**
     * @brief Generate subkeys for each round
     * @param key - Input - key
     * @param subkeys - Output - subkeys
     */
    void generateSubkeys(const uint8_t* key, uint8_t** subkeys);

private:
    /**
     * @brief AES cypher variant
     */
    const AesVariant m_variant;

    /**
     * @brief Executors
     */
    std::vector<std::unique_ptr<Executor>> m_executors;
};
}  // namespace Aes

#endif  // AES_AES_H
