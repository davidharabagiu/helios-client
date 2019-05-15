#include <cstddef>

#include "aes128blockwork.h"
#include "aes128defs.h"

Aes128BlockWork::Aes128BlockWork(const uint8_t* input, const uint8_t** subKeys, uint8_t* output)
    : m_input(input)
    , m_subkeys(subKeys)
    , m_output(output)
{
}

void Aes128BlockWork::addSubkey(uint8_t* state, const uint8_t* subkey) const
{
    auto _state  = reinterpret_cast<uint64_t*>(state);
    auto _subkey = reinterpret_cast<const uint64_t*>(subkey);
    for (size_t i = 0; i < Aes128Defs::kBlockSize / sizeof(uint8_t) * sizeof(uint64_t); ++i)
    {
        _state[i] ^= _subkey[i];
    }
}
