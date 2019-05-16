#ifndef CIPHERFACTORY_H
#define CIPHERFACTORY_H

#include <memory>

#include "cipher.h"

/**
 * @class CipherFactory
 * @brief Creates concrete Cipher implementations
 */
class CipherFactory
{
public:
    /**
     * @brief Cipher type
     */
    enum class Algorithm
    {
        AES128,
        AES192,
        AES256
    };

public:
    /**
     * @brief Destructor
     */
    virtual ~CipherFactory() = default;

    /**
     * @brief Instantiate a Cipher implementation
     * @param cipherType - Specific cipher algorithm
     * @param numberOfThreads - Number of worker threads that can run in parallel
     * @return std::shared_ptr<Cipher>
     */
    virtual std::shared_ptr<Cipher> createCipher(Algorithm cipherType, int numberOfThreads) const = 0;
};

#endif  // CIPHERFACTORY_H
