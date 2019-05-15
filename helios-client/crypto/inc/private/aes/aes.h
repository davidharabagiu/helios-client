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
     * @param numThreads - Number of threads
     */
    Aes(int numThreads);

    /**
     * @brief Run the AES encryption cypher asynchronously
     * @param variant - AES cypher variant
     * @param key - Key, its length depends on variant
     * @param in - Input stream of data
     * @param out - Output stream of data
     * @param callback - Callback
     */
    void encryptAsync(AesVariant variant, uint8_t* key, std::istream& in, std::ostream& out,
                      std::function<void()> callback);

    /**
     * @brief Run the AES decryption cypher asynchronously
     * @param variant - AES cypher variant
     * @param key - Key, its length depends on variant
     * @param in - Input stream of data
     * @param out - Output stream of data
     * @param callback - Callback
     */
    void decryptAsync(AesVariant variant, uint8_t* key, std::istream& in, std::ostream& out,
                      std::function<void()> callback);

private:
    /**
     * @brief Executors
     */
    std::vector<std::unique_ptr<Executor>> m_executors;
};
}  // namespace Aes

#endif  // AES_AES_H
