#include "file.h"

File::File(const std::string& name, const std::string& parentDirectory, bool isDirectory, uint64_t size)
    : m_name(name)
    , m_parentDirectory(parentDirectory)
    , m_isDirectory(isDirectory)
    , m_size(isDirectory ? 0 : size)
    , m_downloadInProgress(false)
    , m_uploadInProgress(false)
    , m_operationProgress(0.0f)
{
    updateFullPath();
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

bool File::downloadInProgress() const
{
    return m_downloadInProgress;
}

bool File::uploadInProgress() const
{
    return m_uploadInProgress;
}

float File::operationProgress() const
{
    return m_operationProgress;
}

void File::setName(const std::string& newVal)
{
    if (m_name != newVal)
    {
        m_name = newVal;
        updateFullPath();
    }
}

void File::setSize(uint64_t newVal)
{
    if (!m_isDirectory)
    {
        m_size = newVal;
    }
}

void File::setDownloadInProgress(bool newVal)
{
    m_downloadInProgress = newVal;
}

void File::setUploadInProgresss(bool newVal)
{
    m_uploadInProgress = newVal;
}

void File::setOperationProgress(float newVal)
{
    m_operationProgress = newVal;
}

void File::updateFullPath()
{
    m_fullPath = m_parentDirectory;
    if (m_fullPath.size() > 0 && m_fullPath[m_fullPath.size() - 1] != '/')
    {
        m_fullPath += '/';
    }
    m_fullPath += m_name;
}
