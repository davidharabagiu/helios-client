#include <random>
#include <limits>
#include <algorithm>
#include <memory>

#include "key.h"
#include "typeconversions.h"

Key::Key(const std::string& name, uint16_t length)
    : m_name(name)
{
    std::random_device                     randomDevice;
    std::mt19937                           mersenneTwister(randomDevice());
    std::uniform_int_distribution<uint8_t> distribution(std::numeric_limits<uint8_t>::min(),
                                                        std::numeric_limits<uint8_t>::max());

    m_content.reserve(length);
    for (uint16_t i = 0; i < length; ++i)
    {
        m_content[i] = distribution(mersenneTwister);
    }
}

Key::Key(const std::string& name, uint16_t length, const uint8_t* content)
    : m_name(name)
    , m_content(content, content + length)
{
}

Key::Key(const Key& rhs)
    : m_name(rhs.m_name)
    , m_content(rhs.m_content)
{
}

Key& Key::operator=(const Key& rhs)
{
    m_name    = rhs.m_name;
    m_content = rhs.m_content;
    return *this;
}

std::string Key::name() const
{
    return m_name;
}

uint16_t Key::length() const
{
    return safe_integral_cast<uint16_t>(m_content.size());
}

const uint8_t* Key::content() const
{
    return m_content.data();
}
