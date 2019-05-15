#ifndef AES128BLOCKWORK_H
#define AES128BLOCKWORK_H

#include <cstdint>

/**
 * @class Aes128BlockWork
 * @brief Functor class represeting a job for processing a 128-bit block of data in the context of the AES-128
 * algorithm. This is a common interface for both the forward and inverse algorithms.
 */
class Aes128BlockWork
{
public:
    /**
     * @brief Constructor
     * @param input - Input block of data (kBlockSize bytes)
     * @param subKeys - One subkey for each round + the initial addition ((kNumberOfRounds + 1) * kBlockSize bytes)
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here once the work is done.
     */
    Aes128BlockWork(const uint8_t* input, const uint8_t** subKeys, uint8_t* output);

    /**
     * @brief Run work
     */
    virtual void operator()() = 0;

protected:
    /**
     * @brief Add (bitwise XOR) the subkey into the state
     * @param state - State
     * @param subkey - Subkey
     */
    void addSubkey(uint8_t* state, const uint8_t* subkey) const;

protected:
    /**
     * @brief Input block
     */
    const uint8_t* m_input;

    /**
     * @brief Subkeys
     */
    const uint8_t** m_subkeys;

    /**
     * @brief Output
     */
    uint8_t* m_output;
};

#endif  // AES128BLOCKWORK_H
