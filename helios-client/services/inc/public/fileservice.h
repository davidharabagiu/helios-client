#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <string>
#include <vector>
#include <memory>

#include "observable.h"
#include "fileservicelistener.h"
#include "file.h"
#include "filetransfer.h"
#include "authenticatedserviceinterface.h"

/**
 * @class FileService
 * @brief Interface for the remote file system management service
 */
class FileService : public AuthenticatedServiceInterface,
                    public Observable<FileServiceListener, ObservableNotifyMode::ASYNC>
{
public:
    /**
     * @brief Returns the current working directory path
     * @return std::string
     */
    virtual std::string currentDirectory() const = 0;

    /**
     * @brief Returns the list of files in the current working directory
     * @return std::vector<std::shared_ptr<const File>>
     */
    virtual std::vector<std::shared_ptr<const File>> files() const = 0;

    /**
     * @brief Returns the list of active file transfers
     * @return std::vector<std::shared_ptr<File>>
     */
    virtual std::vector<std::shared_ptr<FileTransfer>> activeTransfers() const = 0;

    /**
     * @brief Navigate to another directory
     * @param path - New directory path
     * @param True if path is relative to the current working directory
     */
    virtual void changeCurrentDirectory(const std::string& path, bool relative) = 0;

    /**
     * @brief Navigate to the parent directory of the current working directory
     */
    virtual void navigateBack() = 0;

    /**
     * @brief Create a new directory.
     * @param name - Path of the new directory
     * @param relative - True if path is relative to the current working directory
     */
    virtual void createDirectory(const std::string& path, bool relative) = 0;

    /**
     * @brief Upload a local file to the remote file system.
     * @param localPath - Path of the local file
     * @param remotePath - Path of the remote file
     * @param relative - True if the remote path is relative to the current working directory
     * @param encryptionKeyName - Identifier name of the key to use for file encryption
     */
    virtual void uploadFile(const std::string& localPath, const std::string& remotePath, bool relative,
                            const std::string& encryptionKeyName) = 0;

    /**
     * @brief Download a remote file to the local file system. The file will be downloaded at the specified path.
     * @param remotePath - Path of the remote file
     * @param relative - True if the remote path is relative to the current working directory
     * @param localPath - Path of the local file
     * @param decryptionKeyName - Identifier name of the key to user for file decryption
     */
    virtual void downloadFile(const std::string& remotePath, bool relative, const std::string& localPath,
                              const std::string& decryptionKeyName) = 0;

    /**
     * @brief Cancel a download or upload operation. The upload progress made so far won't be discarded.
     * @param filePath - Path of the remote file
     */
    virtual void cancelOperation(const std::string& path) = 0;

    /**
     * @brief Move a file or directory.
     * @param sourcePath - Path of the file or directory to move
     * @param destinationPath - Destination directory path
     */
    virtual void moveFile(const std::string& sourcePath, const std::string& destinationPath) = 0;

    /**
     * @brief Deletes a file or directory from the current working directory. If the target is a directory and it
     * contains another files, the contents will be recursively deleted as well.
     * @param path - Path of the file or directory
     * @param relative - True if the path is relative to the current working directory
     */
    virtual void removeFile(const std::string& path, bool relative) = 0;
};

#endif  // FILESERVICE_H
