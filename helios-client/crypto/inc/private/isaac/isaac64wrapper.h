#ifndef ISAAC_ISAAC64WRAPPER_H
#define ISAAC_ISAAC64WRAPPER_H

#include "random.h"

/**
 * @class Isaac64Wrapper
 * @brief Isaac64 implementation of the Random interface
 */
class Isaac64Wrappwer : public Random
{
public:
    /**
     * @brief Constructor
     */
    Isaac64Wrappwer();

public:  // from Random
    uint64_t get() override;
};

#endif  // ISAAC_ISAAC64WRAPPER_H
