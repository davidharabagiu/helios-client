#ifndef RANDOMFACTORY_H
#define RANDOMFACTORY_H

#include <memory>

// Forward declarations
class Random;

/**
 * @class RandomFactory
 * @brief Abstract factory for Random objects
 */
class RandomFactory
{
public:
    /**
     * @brief Destructor
     */
    virtual ~RandomFactory() = default;

    /**
     * @brief ISAAC64 random engine
     * @return std::shared_ptr<Random>
     */
    virtual std::shared_ptr<Random> isaac64() const = 0;
};

#endif  // RANDOMFACTORY_H
