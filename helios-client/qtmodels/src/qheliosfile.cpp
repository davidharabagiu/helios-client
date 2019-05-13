#include "qheliosfile.h"
#include "file.h"

QHeliosFile::QHeliosFile(const std::shared_ptr<const File>& data)
    : m_data(data)
{
}

bool QHeliosFile::isValid() const
{
    return static_cast<bool>(m_data);
}

QString QHeliosFile::name() const
{
    return isValid() ? QString::fromStdString(m_data->name()) : QString();
}

QString QHeliosFile::fullPath() const
{
    return isValid() ? QString::fromStdString(m_data->fullPath()) : QString();
}

bool QHeliosFile::isDirectory() const
{
    return isValid() ? m_data->isDirectory() : false;
}

quint64 QHeliosFile::size() const
{
    return isValid() ? m_data->size() : 0;
}
