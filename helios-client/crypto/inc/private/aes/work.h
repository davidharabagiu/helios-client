#ifndef AES_WORK_H
#define AES_WORK_H

#include <cstdint>
#include <cstddef>

namespace Aes
{
/**
 * @class Work
 * @brief Functor class represeting a job for processing a 128-bit block of data. This is a common interface for both
 * the forward and inverse algorithms.
 */
class Work
{
public:
    /**
     * @brief Constructor
     * @param input - Input block of data (kBlockSize bytes)
     * @param rounds - Number of rounds
     * @param roundKeys - Round keys (numberOfRounds * kBlockSize bytes)
     * @param output - Output block of data (kBlockSize bytes). The output will be stored here once the work is done.
     */
    Work(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output);

    /**
     * @brief Run work
     */
    virtual void operator()() = 0;

protected:
    /**
     * @brief Add (bitwise XOR) the round key into the state
     * @param state - State
     * @param roundKeyIndex - Index of the round key int m_roundKeys
     */
    void addRoundKey(uint8_t* state, size_t roundKeyIndex) const;

protected:
    /**
     * @brief Number of rounds
     */
    const size_t m_kRounds;

    /**
     * @brief Input block
     */
    const uint8_t* m_input;

    /**
     * @brief Round keys
     */
    const uint8_t* m_roundKeys;

    /**
     * @brief Output
     */
    uint8_t* m_output;
};
}  // namespace Aes

#endif  // AES_WORK_H
