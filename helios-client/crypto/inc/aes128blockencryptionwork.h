#ifndef AES128BLOCKENCRYPTIONWORK_H
#define AES128BLOCKENCRYPTIONWORK_H

#include "aes128blockwork.h"

/**
 * @class Aes128BlockEncryptionWork
 * @brief Implementation of Aes128BlockWork for the encryption algoritgm
 */
class Aes128BlockEncryptionWork : public Aes128BlockWork
{
public:
    /**
     * @brief Constructor. Forwards arguments to Aes128BlockWork.
     */
    Aes128BlockEncryptionWork(const uint8_t* input, const uint8_t** subKeys, uint8_t* output);

public:  // from Aes128BlockWork
    void operator()() override;
};

#endif  // AES128BLOCKENCRYPTIONWORK_H
