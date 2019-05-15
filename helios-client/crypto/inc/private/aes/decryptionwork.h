#ifndef AES_DECRYPTIONWORK_H
#define AES_DECRYPTIONWORK_H

#include "aes/work.h"

namespace Aes
{
/**
 * @class DecryptionWork
 * @brief Implementation of Work for the decryption cypher
 */
class DecryptionWork : public Work
{
public:
    /**
     * @brief Constructor. Forwards arguments to Work.
     */
    DecryptionWork(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output);

public:  // from Work
    void operator()() override;
};
}  // namespace Aes

#endif  // AES_DECRYPTIONWORK_H
