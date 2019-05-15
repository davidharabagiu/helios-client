#ifndef AES_ENCRYPTIONWORK_H
#define AES_ENCRYPTIONWORK_H

#include "aes/work.h"

namespace Aes
{
/**
 * @class EncryptionWork
 * @brief Implementation of Work for the encryption cypher
 */
class EncryptionWork : public Work
{
public:
    /**
     * @brief Constructor. Forwards arguments to Work.
     */
    EncryptionWork(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output);

public:  // from Work
    void operator()() override;
};
}  // namespace Aes

#endif  // AES_ENCRYPTIONWORK_H
