#ifndef QREMOTEFILESYSTEMCONTROLLERIMPL_H
#define QREMOTEFILESYSTEMCONTROLLERIMPL_H

#include <QString>
#include <QList>
#include <QVariant>
#include <memory>
#include "fileservicelistener.h"

#include "qusersession.h"

// Forward declarations
class QRemoteFileSystemController;
class FileService;
class QFileTransfer;

/**
 * @class QRemoteFileSystemControllerImpl
 * @brief Private implementation of QRemoteFileSystemController
 */
class QRemoteFileSystemControllerImpl : public FileServiceListener,
                                        public std::enable_shared_from_this<QRemoteFileSystemControllerImpl>
{
public:
    /**
     * @brief Constructor
     * @param publicImpl - Public implementation
     */
    QRemoteFileSystemControllerImpl(QRemoteFileSystemController* publicImpl);

    /**
     * @brief Register for notifications from internal services
     */
    void registerForNotifications();

    /**
     * @brief Unregister from notifications from internal services
     */
    void unregisterFromNotifications();

public:  // forwarded from QRemoteFileSystemController
    void         setSession(const QUserSession& newVal);
    void         resetSession();
    QString      cwd() const;
    void         setCwd(const QString& newValue);
    QVariantList files() const;
    QVariantList transfers() const;
    void         openDirectory(const QString& dirName);
    void         goBack();
    void         createDirectory(const QString& dirName);
    void         remove(const QString& fileName);
    void         move(const QString& fileName, const QString& destinationDirPath);
    void         rename(const QString& fileName, const QString& newName);
    void         upload(const QUrl& localPath, const QString& encryptionKeyName);
    void         download(const QString& filename, const QUrl& destinationDir, const QString& decryptionKeyName);
    void         cancelTransfer(const QFileTransfer& transfer);
    void         shareFile(const QString& username, const QString& fileName);
    void         acceptSharedFile(const QString& notificationId, const QString& fileName);

public:  // from FileServiceListener
    void currentDirectoryChanged() override;
    void directoryCreated(std::shared_ptr<const File> directory) override;
    void fileMoved(std::shared_ptr<const File> oldFile, std::shared_ptr<const File> file) override;
    void fileRemoved(std::shared_ptr<const File> file) override;
    void uploadedFileInCurrentDir(std::shared_ptr<const File> file) override;
    void transferStarted(std::shared_ptr<FileTransfer> transfer) override;
    void transferProgressChanged(std::shared_ptr<FileTransfer> transfer) override;
    void transferCompleted(std::shared_ptr<FileTransfer> transfer) override;
    void transferAborted(std::shared_ptr<FileTransfer> transfer) override;
    void fileShared() override;
    void acceptedFileShare(std::shared_ptr<const File> file) override;
    void errorOccured(const std::string& errorString) override;

private:
    /**
     * @brief Public implementation
     */
    QRemoteFileSystemController* m_publicImpl;

    /**
     * @brief Current list of files
     */
    QVariantList m_files;

    /**
     * @brief Current list of file transfers
     */
    QVariantList m_transfers;

    /**
     * @brief File service instance
     */
    std::shared_ptr<FileService> m_fileService;
};

#endif  // QREMOTEFILESYSTEMCONTROLLERIMPL_H
