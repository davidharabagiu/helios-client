#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <string>
#include <cstdint>

/**
 * @class FileTransfer
 * @brief Models a file transfer
 */
class FileTransfer
{
public:  // enums
    /**
     * @brief File transfer mode
     */
    enum class TransferMode
    {
        UPLOAD,
        DOWNLOAD
    };

public:
    /**
     * @brief Constructor
     * @param localPath - Path of the file in the local file system
     * @param remotePath - Path of the file in the remote file system
     * @param mode - Transfer mode
     * @param fileSize - File size in bytes
     */
    FileTransfer(const std::string& localPath, const std::string remotePath, TransferMode mode, uint64_t fileSize);

    /**
     * @brief Getter for m_localPath
     * @return std::string
     */
    std::string localPath() const;

    /**
     * @brief Getter for m_remotePath
     * @return std::string
     */
    std::string remotePath() const;

    /**
     * @brief Getter for m_mode
     * @return TransferMode
     */
    TransferMode mode() const;

    /**
     * @brief Getter for m_fileSize
     * @return uint64_t
     */
    uint64_t fileSize() const;

    /**
     * @brief Getter for m_transferredBytes
     * @return uint64_t
     */
    uint64_t transferredBytes() const;

    /**
     * @brief Setter for m_transferredBytes
     * @param newVal - uint64_t
     */
    void setTransferredBytes(uint64_t newVal);

    /**
     * @brief Getter for m_speed
     * @return uint64_t
     */
    uint64_t speed() const;

    /**
     * @brief Setter for m_spped
     * @param newVal - uint64_t
     */
    void setSpeed(uint64_t newVal);

private:
    /**
     * @brief Local path of the transferred file
     */
    std::string m_localPath;

    /**
     * @brief Remote path of the transferred file
     */
    std::string m_remotePath;

    /**
     * @brief Transfer mode
     */
    TransferMode m_mode;

    /**
     * @brief Transferred bytes
     */
    uint64_t m_transferredBytes;

    /**
     * @brief Total bytes
     */
    uint64_t m_fileSize;

    /**
     * @brief Calculated transfer speed represented in bytes per second
     */
    uint64_t m_speed;
};

#endif  // FILETRANSFER_H
