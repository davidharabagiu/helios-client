#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <string>
#include <vector>

#include "serviceinterface.h"
#include "observable.h"
#include "fileservicelistener.h"
#include "file.h"

/**
 * @class FileService
 * @brief Interface for the remote file system management service
 */
class FileService : public ServiceInterface, public Observable<FileSeriveListener>
{
public:
    /**
     * @brief Destructor
     */
    virtual ~FileService() = default;

    /**
     * @brief Set the authentication token of the working user. Having a valid auth token is a precondition for all
     * operations to work.
     * @param authToken - Authentication token
     */
    virtual void setAuthToken(const std::string& authToken) = 0;

    /**
     * @brief Removed the current authentication token. This will disable all operations.
     */
    virtual void removeAuthToken() = 0;

    /**
     * @brief Returns the current working directory path
     * @return std::string
     */
    virtual std::string currentDirectory() const = 0;

    /**
     * @brief Returns the list of files in the current working directory
     * @return std::vector<File>
     */
    virtual std::vector<File> files() const = 0;

    /**
     * @brief Navigate to another directory
     * @param path - New directory path
     */
    virtual void changeCurrentDirectory(const std::string& path) = 0;

    /**
     * @brief Create a new directory in the current working directory.
     * @param name - Name of the new directory
     */
    virtual void createDirectory(const std::string& name) = 0;

    /**
     * @brief Upload a local file to the remote file system. The file will be uploaded in the current working directory
     * under the same name as the local file.
     * @param localPath - Path of the local file
     */
    virtual void uploadFile(const std::string& localPath) = 0;

    /**
     * @brief Download a remote file from the current working directory to the local file system. The file will be
     * downloaded in the specified path under the same name as the remote file.
     * @param name - Name of the remote file
     * @param localPath - Directory path of the download destintion
     */
    virtual void downloadFile(const std::string& name, const std::string& localPath) = 0;

    /**
     * @brief Cancel a download or upload operation. The upload progress made so far won't be discarded.
     * @param filePath - Full path of the remote file
     */
    virtual void cancelOperation(const std::string& filePath) = 0;

    /**
     * @brief Move a file or directory from the current working directory to another directory.
     * @param name - Name of the file
     * @param newParentDirectory - Destination directory path
     */
    virtual void moveFile(const std::string& name, const std::string& newParentDirectory) = 0;

    /**
     * @brief Rename a file or directory from the current working directory.
     * @param name - Name of the file
     * @param newName - New name of the file
     */
    virtual void renameFile(const std::string& name, const std::string& newName) = 0;

    /**
     * @brief Deletes a file or directory from the current working directory. If the target is a directory and it
     * contains another files, the contents will be recursively deleted as well.
     * @param name - Name of the file
     */
    virtual void removeFile(const std::string& name) = 0;
}

#endif  // FILESERVICE_H
