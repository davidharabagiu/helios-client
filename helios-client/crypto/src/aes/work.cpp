#include "aes/work.h"
#include "aes/commondefs.h"

using namespace Aes;

Work::Work(const uint8_t* input, size_t rounds, const uint8_t* roundKeys, uint8_t* output)
    : m_kRounds(rounds)
    , m_input(input)
    , m_roundKeys(roundKeys)
    , m_output(output)
{
}

void Work::addRoundKey(uint8_t* state, size_t roundKeyIndex) const
{
    auto _state    = reinterpret_cast<uint64_t*>(state);
    auto _roundKey = reinterpret_cast<const uint64_t*>(m_roundKeys + roundKeyIndex * kBlockSize);
    for (size_t i = 0; i < kBlockSize / sizeof(uint64_t); ++i)
    {
        _state[i] ^= _roundKey[i];
    }
}
