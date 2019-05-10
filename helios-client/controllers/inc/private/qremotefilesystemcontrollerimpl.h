#ifndef QREMOTEFILESYSTEMCONTROLLERIMPL_H
#define QREMOTEFILESYSTEMCONTROLLERIMPL_H

#include <memory>
#include "fileservicelistener.h"

// Forward declarations
class QRemoteFileSystemController;
class FileService;

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

public:  // from FileServiceListener
    void fileServiceEnabled() override;
    void currentDirectoryChanged() override;
    void directoryCreated(const std::string& path) override;
    void fileMoved(const std::string& sourcePath, const std::string& destinationPath) override;
    void fileRemoved(const std::string& path) override;
    void fileDownloadStarted(const std::string& path) override;
    void fileUploadStarted(const std::string& path) override;
    void fileOperationProgressChanged(const std::string& path) override;
    void fileOperationCompleted(const std::string& path) override;
    void fileOperationAborted(const std::string& path) override;
    void errorOccured(const std::string& errorString) override;

private:
    /**
     * @brief Public implementation
     */
    QRemoteFileSystemController* m_publicImpl;

    /**
     * @brief File service instance
     */
    std::shared_ptr<FileService> m_fileService;
};

#endif  // QREMOTEFILESYSTEMCONTROLLERIMPL_H
