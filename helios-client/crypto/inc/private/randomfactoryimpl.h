#ifndef RANDOMFACTORYIMPL_H
#define RANDOMFACTORYIMPL_H

#include "randomfactory.h"

/**
 * @class RandomFactoryImpl
 * @brief Implementation of RandomFactory
 */
class RandomFactoryImpl : public RandomFactory
{
public:
    /**
     * @brief Constructor
     */
    RandomFactoryImpl() = default;

public:  // From RandomFactory
    std::shared_ptr<Random> isaac64() const override;
};

#endif  // RANDOMFACTORYIMPL_H
