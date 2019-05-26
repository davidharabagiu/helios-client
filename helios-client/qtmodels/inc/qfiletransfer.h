#ifndef QFILETRANSFER_H
#define QFILETRANSFER_H

#include <QMetaType>
#include <QString>
#include <memory>

#include "filetransfer.h"

/**
 * @class QFileTransfer
 * @brief Qt wrapper over FileTransfer
 */
class QFileTransfer
{
    Q_GADGET

    /**
     * @property localPath
     * @brief Local path of the transferred file
     */
    Q_PROPERTY(QString localPath READ localPath)

    /**
     * @property remotePath
     * @brief Local path of the transferred file
     */
    Q_PROPERTY(QString remotePath READ remotePath)

    /**
     * @property transferMode
     * @brief Upload / download
     */
    Q_PROPERTY(QTransferMode mode READ mode)

    /**
     * @property fileSize
     * @brief File size in bytes
     */
    Q_PROPERTY(quint64 fileSize READ fileSize)

    /**
     * @property transferredBytes
     * @brief Number of transferred bytes
     */
    Q_PROPERTY(quint64 transferredBytes READ transferredBytes)

public:  // Enums
    /**
     * @brief File transfer mode
     */
    enum QTransferMode
    {
        UPLOAD   = static_cast<int>(FileTransfer::TransferMode::UPLOAD),
        DOWNLOAD = static_cast<int>(FileTransfer::TransferMode::DOWNLOAD)
    };
    Q_ENUM(QTransferMode)

public:
    /**
     * @brief Constructs an invalid QFileTransfer
     */
    QFileTransfer() = default;

    /**
     * @brief Constructor
     * @param data - Internal data pointer
     */
    QFileTransfer(const std::shared_ptr<const FileTransfer>& data);

    /**
     * @brief Getter for isValid
     * @return bool
     */
    bool isValid() const;

    /**
     * @brief Returns a pointer to the internal data
     * @return std::shared_ptr<const FileTransfer>
     */
    std::shared_ptr<const FileTransfer> data() const;

public:  // Wrappers over FileTransfer methods
    QString       localPath() const;
    QString       remotePath() const;
    QTransferMode mode() const;
    quint64       fileSize() const;
    quint64       transferredBytes() const;

private:
    /**
     * @brief Internal data
     */
    std::shared_ptr<const FileTransfer> m_data;
};

Q_DECLARE_METATYPE(QFileTransfer)

#endif  // QFILETRANSFER_H
