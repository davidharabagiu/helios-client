#include <algorithm>

#include "aes/decryptionwork.h"
#include "aes/tables.h"
#include "aes/commondefs.h"

using namespace Aes;

DecryptionWork::DecryptionWork(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output)
    : Work(input, rounds, roundKeys, output)
{
}

void DecryptionWork::operator()()
{
    uint8_t state[kBlockSize];
    std::copy_n(m_input, kBlockSize, state);

    size_t round = m_kRounds;

    // AddRoundKey
    addRoundKey(state, round);

    // InvShiftRows + InvSubBytes
    {
        uint8_t tmp;

        // Row 0
        state[0]  = kInvSbox[state[0]];
        state[4]  = kInvSbox[state[4]];
        state[8]  = kInvSbox[state[8]];
        state[12] = kInvSbox[state[12]];

        // Row 1
        tmp       = kInvSbox[state[13]];
        state[13] = kInvSbox[state[9]];
        state[9]  = kInvSbox[state[5]];
        state[5]  = kInvSbox[state[1]];
        state[1]  = tmp;

        // Row 2
        tmp       = kInvSbox[state[2]];
        state[2]  = kInvSbox[state[10]];
        state[10] = tmp;
        tmp       = kInvSbox[state[6]];
        state[6]  = kInvSbox[state[14]];
        state[14] = tmp;

        // Row 3
        tmp       = kInvSbox[state[3]];
        state[3]  = kInvSbox[state[7]];
        state[7]  = kInvSbox[state[11]];
        state[11] = kInvSbox[state[15]];
        state[15] = tmp;
    }

    while (round--)
    {
        // AddRoundKey
        addRoundKey(state, round);

        if (round != 0)
        {
            // InvMixColumns + InvShiftRows + InvSubBytes
            uint8_t nextState[kBlockSize];

            // Column 0
            nextState[0]  = kXtimesE[state[0]] ^ kXtimesB[state[1]] ^ kXtimesD[state[2]] ^ kXtimes9[state[3]];
            nextState[5]  = kXtimes9[state[0]] ^ kXtimesE[state[1]] ^ kXtimesB[state[2]] ^ kXtimesD[state[3]];
            nextState[10] = kXtimesD[state[0]] ^ kXtimes9[state[1]] ^ kXtimesE[state[2]] ^ kXtimesB[state[3]];
            nextState[15] = kXtimesB[state[0]] ^ kXtimesD[state[1]] ^ kXtimes9[state[2]] ^ kXtimesE[state[3]];

            // Column 1
            nextState[4]  = kXtimesE[state[4]] ^ kXtimesB[state[5]] ^ kXtimesD[state[6]] ^ kXtimes9[state[7]];
            nextState[9]  = kXtimes9[state[4]] ^ kXtimesE[state[5]] ^ kXtimesB[state[6]] ^ kXtimesD[state[7]];
            nextState[14] = kXtimesD[state[4]] ^ kXtimes9[state[5]] ^ kXtimesE[state[6]] ^ kXtimesB[state[7]];
            nextState[3]  = kXtimesB[state[4]] ^ kXtimesD[state[5]] ^ kXtimes9[state[6]] ^ kXtimesE[state[7]];

            // Column 2
            nextState[8]  = kXtimesE[state[8]] ^ kXtimesB[state[9]] ^ kXtimesD[state[10]] ^ kXtimes9[state[11]];
            nextState[13] = kXtimes9[state[8]] ^ kXtimesE[state[9]] ^ kXtimesB[state[10]] ^ kXtimesD[state[11]];
            nextState[2]  = kXtimesD[state[8]] ^ kXtimes9[state[9]] ^ kXtimesE[state[10]] ^ kXtimesB[state[11]];
            nextState[7]  = kXtimesB[state[8]] ^ kXtimesD[state[9]] ^ kXtimes9[state[10]] ^ kXtimesE[state[11]];

            // Column 3
            nextState[12] = kXtimesE[state[12]] ^ kXtimesB[state[13]] ^ kXtimesD[state[14]] ^ kXtimes9[state[15]];
            nextState[1]  = kXtimes9[state[12]] ^ kXtimesE[state[13]] ^ kXtimesB[state[14]] ^ kXtimesD[state[15]];
            nextState[6]  = kXtimesD[state[12]] ^ kXtimes9[state[13]] ^ kXtimesE[state[14]] ^ kXtimesB[state[15]];
            nextState[11] = kXtimesB[state[12]] ^ kXtimesD[state[13]] ^ kXtimes9[state[14]] ^ kXtimesE[state[15]];

            for (size_t i = 0; i < kBlockSize; i++)
                state[i] = kInvSbox[nextState[i]];
        }
    }

    std::copy_n(state, kBlockSize, m_output);
}
