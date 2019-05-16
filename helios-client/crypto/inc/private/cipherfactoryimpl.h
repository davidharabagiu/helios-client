#ifndef CIPHERFACTORYIMPL_H
#define CIPHERFACTORYIMPL_H

#include "cipherfactory.h"

/**
 * @class CipherFactoryImpl
 * @brief Private implementation of CipherFactory
 */
class CipherFactoryImpl : public CipherFactory
{
public:
    /**
     * @brief Constructor
     */
    CipherFactoryImpl() = default;

public:  // from CipherFactoryImpl
    std::shared_ptr<Cipher> createCipher(Algorithm cipherType, int numberOfThreads) const override;
};

#endif  // CIPHERFACTORYIMPL_H
