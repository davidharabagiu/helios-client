#include <algorithm>

#include "aes/encryptionwork.h"
#include "aes/tables.h"

using namespace Aes;

EncryptionWork::EncryptionWork(AesVariant variant, const uint8_t* input, const uint8_t** subKeys, uint8_t* output)
    : Work(variant, input, subKeys, output)
{
}

void EncryptionWork::operator()()
{
    uint8_t state[kBlockSize];
    std::copy_n(m_input, kBlockSize, state);

    // AddRoundKey
    addSubkey(state, m_subkeys[0]);

    for (int round = 1; round < m_numberOfRounds + 1; ++round)
    {
        if (round < m_numberOfRounds)
        {
            // SubBytes + ShiftRows + MixColumns
            uint8_t nextState[kBlockSize];

            nextState[0]  = kXtimes2Sbox[state[0]] ^ kXtimes3Sbox[state[5]] ^ kSbox[state[10]] ^ kSbox[state[15]];
            nextState[1]  = kSbox[state[0]] ^ kXtimes2Sbox[state[5]] ^ kXtimes3Sbox[state[10]] ^ kSbox[state[15]];
            nextState[2]  = kSbox[state[0]] ^ kSbox[state[5]] ^ kXtimes2Sbox[state[10]] ^ kXtimes3Sbox[state[15]];
            nextState[3]  = kXtimes3Sbox[state[0]] ^ kSbox[state[5]] ^ kSbox[state[10]] ^ kXtimes2Sbox[state[15]];
            nextState[4]  = kXtimes2Sbox[state[4]] ^ kXtimes3Sbox[state[9]] ^ kSbox[state[14]] ^ kSbox[state[3]];
            nextState[5]  = kSbox[state[4]] ^ kXtimes2Sbox[state[9]] ^ kXtimes3Sbox[state[14]] ^ kSbox[state[3]];
            nextState[6]  = kSbox[state[4]] ^ kSbox[state[9]] ^ kXtimes2Sbox[state[14]] ^ kXtimes3Sbox[state[3]];
            nextState[7]  = kXtimes3Sbox[state[4]] ^ kSbox[state[9]] ^ kSbox[state[14]] ^ kXtimes2Sbox[state[3]];
            nextState[8]  = kXtimes2Sbox[state[8]] ^ kXtimes3Sbox[state[13]] ^ kSbox[state[2]] ^ kSbox[state[7]];
            nextState[9]  = kSbox[state[8]] ^ kXtimes2Sbox[state[13]] ^ kXtimes3Sbox[state[2]] ^ kSbox[state[7]];
            nextState[10] = kSbox[state[8]] ^ kSbox[state[13]] ^ kXtimes2Sbox[state[2]] ^ kXtimes3Sbox[state[7]];
            nextState[11] = kXtimes3Sbox[state[8]] ^ kSbox[state[13]] ^ kSbox[state[2]] ^ kXtimes2Sbox[state[7]];
            nextState[12] = kXtimes2Sbox[state[12]] ^ kXtimes3Sbox[state[1]] ^ kSbox[state[6]] ^ kSbox[state[11]];
            nextState[13] = kSbox[state[12]] ^ kXtimes2Sbox[state[1]] ^ kXtimes3Sbox[state[6]] ^ kSbox[state[11]];
            nextState[14] = kSbox[state[12]] ^ kSbox[state[1]] ^ kXtimes2Sbox[state[6]] ^ kXtimes3Sbox[state[11]];
            nextState[15] = kXtimes3Sbox[state[12]] ^ kSbox[state[1]] ^ kSbox[state[6]] ^ kXtimes2Sbox[state[11]];

            std::copy_n(nextState, kBlockSize, state);
        }
        else
        {
            // SubBytes + ShiftRows
            uint8_t tmp;

            // Row 0 (Substitute)
            state[0]  = kSbox[state[0]];
            state[4]  = kSbox[state[4]];
            state[8]  = kSbox[state[8]];
            state[12] = kSbox[state[12]];

            // Row 1 (Substitute + Rotate)
            tmp       = kSbox[state[1]];
            state[1]  = kSbox[state[5]];
            state[5]  = kSbox[state[9]];
            state[9]  = kSbox[state[13]];
            state[13] = tmp;

            // Row 2 (Substitute + Rotate)
            tmp       = kSbox[state[2]];
            state[2]  = kSbox[state[10]];
            state[10] = tmp;
            tmp       = kSbox[state[6]];
            state[6]  = kSbox[state[14]];
            state[14] = tmp;

            // Row 3 (Substitute + Rotate)
            tmp       = kSbox[state[15]];
            state[15] = kSbox[state[11]];
            state[11] = kSbox[state[7]];
            state[7]  = kSbox[state[3]];
            state[3]  = tmp;
        }

        // AddRoundKey
        addSubkey(state, m_subkeys[round]);
    }

    std::copy_n(state, kBlockSize, m_output);
}
