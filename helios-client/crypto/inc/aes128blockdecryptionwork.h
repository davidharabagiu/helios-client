#ifndef AES128BLOCKDECRYPTIONWORK_H
#define AES128BLOCKDECRYPTIONWORK_H

#include "aes128blockwork.h"

/**
 * @class Aes128BlockDecryptionWork
 * @brief Implementation of Aes128BlockWork for the decryption algoritgm
 */
class Aes128BlockDecryptionWork : public Aes128BlockWork
{
public:
    /**
     * @brief Constructor. Forwards arguments to Aes128BlockWork.
     */
    Aes128BlockDecryptionWork(const uint8_t* input, const uint8_t** subKeys, uint8_t* output);

public:  // from Aes128BlockWork
    void operator()() override;
};

#endif  // AES128BLOCKDECRYPTIONWORK_H
