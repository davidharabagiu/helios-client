#include "qfiletransfer.h"

QFileTransfer::QFileTransfer(const std::shared_ptr<const FileTransfer>& data)
    : m_data(data)
{
}

bool QFileTransfer::isValid() const
{
    return static_cast<bool>(m_data);
}

std::shared_ptr<const FileTransfer> QFileTransfer::data() const
{
    return m_data;
}

QString QFileTransfer::localPath() const
{
    return isValid() ? QString::fromStdString(m_data->localPath()) : QString();
}

QString QFileTransfer::remotePath() const
{
    return isValid() ? QString::fromStdString(m_data->remotePath()) : QString();
}

QFileTransfer::QTransferMode QFileTransfer::mode() const
{
    return isValid() ? static_cast<QTransferMode>(m_data->mode()) : static_cast<QTransferMode>(0);
}

quint64 QFileTransfer::fileSize() const
{
    return isValid() ? m_data->fileSize() : 0;
}

quint64 QFileTransfer::transferredBytes() const
{
    return isValid() ? m_data->transferredBytes() : 0;
}

quint64 QFileTransfer::speed() const
{
    return isValid() ? m_data->speed() : 0;
}
