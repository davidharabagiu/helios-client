#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

/**
 * @class Random
 * @brief Random number generator interface
 */
class Random
{
public:
    /**
     * @brief Destructor
     */
    virtual ~Random() = default;

    /**
     * @brief Get a random number
     * @return uint64_t
     */
    virtual uint64_t get() = 0;
};

#endif  // RANDOM_H
