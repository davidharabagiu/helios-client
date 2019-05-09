#ifndef FILESERVICEIMPL_H
#define FILESERVICEIMPL_H

#include <string>
#include <map>
#include <mutex>
#include <memory>

#include "fileservice.h"
#include "fileapicaller.h"
#include "file.h"
#include "filetransfer.h"
#include "executor.h"

// Forward declarations
class Config;

/**
 * @class FileServiceImpl
 * @brief Private implementation of FileService
 */
class FileServiceImpl : public FileService
{
private:
    /**
     * @struct FileTransferInternal
     * @brief A file transfer with an associated file stream
     */
    struct FileTransferInternal
    {
        /**
         * @brief Public transfer representation
         */
        std::shared_ptr<FileTransfer> transfer;

        /**
         * @brief Input file stream. Can be either std::ofstream or std::ifstream.
         */
        std::shared_ptr<void> stream;
    };

public:
    /**
     * @brief Constructor
     * @param config - Config instance
     * @param fileApiCaller - File API caller instance
     */
    FileServiceImpl(std::shared_ptr<Config> config, std::unique_ptr<FileApiCaller> fileApiCaller);

public:  // from FileService
    bool                                       enabled() const override;
    void                                       setAuthToken(const std::string& authToken) override;
    void                                       removeAuthToken() override;
    std::string                                currentDirectory() const override;
    std::vector<std::shared_ptr<const File>>   files() const override;
    std::vector<std::shared_ptr<FileTransfer>> activeTransfers() const override;
    void                                       changeCurrentDirectory(const std::string& path, bool relative) override;
    void                                       createDirectory(const std::string& path, bool relative) override;
    void uploadFile(const std::string& localPath, const std::string& remotePath, bool relative) override;
    void downloadFile(const std::string& remotePath, bool relative, const std::string& localPath) override;
    void cancelOperation(const std::string& path) override;
    void moveFile(const std::string& sourcePath, const std::string& destinationPath) override;
    void removeFile(const std::string& path, bool relative) override;

private:
    /**
     * @brief Collect a list of files returned from an API list call in m_files
     * @param files - List of files
     */
    void collectApiFileList(const std::vector<std::tuple<std::string, bool>>& files);

    /**
     * @brief Concatenate a base with a relative path to obtain the full path
     * @param base - Base path
     * @param relativePath - Relative path
     * @return Full path
     */
    static std::string concatenatePaths(const std::string& base, const std::string& relativePath);

    /**
     * @brief Get the file name and parent directory from its full path
     * @param path - Full path
     * @param name - Out param, name of the file
     * @param parent - Out param, parent directory of the file
     */
    static void getFileNameAndParentDir(const std::string& path, std::string& name, std::string& parent);

    /**
     * @brief Remove the trailing slash from a path if it exists
     * @param path - Path
     */
    static void removeTrailingSlash(std::string& path);

private:
    /**
     * @brief File operations API caller instance
     */
    std::unique_ptr<FileApiCaller> m_apiCaller;

    /**
     * @brief Config instance
     */
    std::shared_ptr<Config> m_config;

    /**
     * @brief Authentication token
     */
    std::string m_authToken;

    /**
     * @brief Current working directory
     */
    std::string m_currentDirectory;

    /**
     * @brief Files in the current working directory mapped by their name
     */
    std::map<std::string, std::shared_ptr<const File>> m_files;

    /**
     * @brief Currently active file transfers mapped by the remote file path
     */
    std::map<std::string, std::shared_ptr<FileTransferInternal>> m_activeTransfers;

    /**
     * @brief Executor for async file transfers
     */
    Executor m_transfersExecutor;

    /**
     * @brief Operations mutex
     */
    mutable std::mutex m_mutex;
};

#endif  // FILESERVICEIMPL_H
