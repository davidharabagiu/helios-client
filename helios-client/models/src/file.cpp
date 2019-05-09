#include "file.h"

File::File(const std::string& name, const std::string& parentDirectory, bool isDirectory, uint64_t size)
    : m_name(name)
    , m_parentDirectory(parentDirectory)
    , m_isDirectory(isDirectory)
    , m_size(isDirectory ? 0 : size)
{
    m_fullPath = m_parentDirectory;
    if (m_fullPath.size() > 0 && m_fullPath[m_fullPath.size() - 1] != '/')
    {
        m_fullPath += '/';
    }
    m_fullPath += m_name;
}

std::string File::name() const
{
    return m_name;
}

std::string File::fullPath() const
{
    return m_fullPath;
}

bool File::isDirectory() const
{
    return m_isDirectory;
}

uint64_t File::size() const
{
    return m_size;
}
