#ifndef QFILETRANSFER_H
#define QFILETRANSFER_H

#include <QMetaType>
#include <QString>
#include <memory>

#include "filetransfer.h"

class QFileTransfer
{
    Q_GADGET

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
