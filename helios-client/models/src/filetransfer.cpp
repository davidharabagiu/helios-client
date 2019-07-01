#include "filetransfer.h"

FileTransfer::FileTransfer(const std::string& localPath, const std::string remotePath, TransferMode mode,
                           uint64_t fileSize)
    : m_localPath(localPath)
    , m_remotePath(remotePath)
    , m_mode(mode)
    , m_transferredBytes(0)
    , m_fileSize(fileSize)
    , m_speed(0)
{
}

std::string FileTransfer::localPath() const
{
    return m_localPath;
}

std::string FileTransfer::remotePath() const
{
    return m_remotePath;
}

FileTransfer::TransferMode FileTransfer::mode() const
{
    return m_mode;
}

uint64_t FileTransfer::fileSize() const
{
    return m_fileSize;
}

uint64_t FileTransfer::transferredBytes() const
{
    return m_transferredBytes;
}

void FileTransfer::setTransferredBytes(uint64_t newVal)
{
    m_transferredBytes = newVal;
}

uint64_t FileTransfer::speed() const
{
    return m_speed;
}

void FileTransfer::setSpeed(uint64_t newVal)
{
    m_speed = newVal;
}
