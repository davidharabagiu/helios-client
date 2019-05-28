#ifndef FILESERVICEIMPL_H
#define FILESERVICEIMPL_H

#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <vector>

#include "fileservice.h"
#include "fileapicaller.h"
#include "file.h"
#include "filetransfer.h"
#include "executor.h"
#include "cipherfactory.h"

// Forward declarations
class Config;
class KeyManager;

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

        /**
         * @brief Set this to true in order to cancel a transfer
         */
        bool canceled = false;
    };

public:
    /**
     * @brief Constructor
     * @param config - Config instance
     * @param fileApiCaller - File API caller instance
     * @param cipherFactory - Cipher factory instance
     * @param keyManager - Key manager instance
     */
    FileServiceImpl(std::shared_ptr<Config> config, std::unique_ptr<FileApiCaller> fileApiCaller,
                    std::unique_ptr<CipherFactory> cipherFactory, std::shared_ptr<KeyManager> keyManager);

    /**
     * @brief Destructor
     */
    ~FileServiceImpl();

public:  // from AuthenticatedService
    void setSession(const UserSession& authToken) override;
    void removeSession() override;

public:  // from FileService
    std::string                                currentDirectory() const override;
    std::vector<std::shared_ptr<const File>>   files() const override;
    std::vector<std::shared_ptr<FileTransfer>> activeTransfers() const override;
    void                                       changeCurrentDirectory(const std::string& path, bool relative) override;
    void                                       navigateBack() override;
    void                                       createDirectory(const std::string& path, bool relative) override;
    void uploadFile(const std::string& localPath, const std::string& remotePath, bool relative,
                    const std::string& encryptionKeyName) override;
    void downloadFile(const std::string& remotePath, bool relative, const std::string& localPath,
                      const std::string& decryptionKeyName) override;
    void cancelOperation(const std::string& path) override;
    void moveFile(const std::string& sourcePath, const std::string& destinationPath) override;
    void removeFile(const std::string& path, bool relative) override;

private:
    /**
     * @brief Collect a list of files returned from an API list call in m_files
     * @param files - List of files
     */
    void collectApiFileList(const std::vector<std::tuple<std::string, bool, std::optional<uint64_t>>>& files);

    /**
     * @brief Move operation completion
     * @param source - Source path
     * @param destination - Destination path
     * @param isDir - isDirectory
     * @param size - Size in bytes if it's not a directory
     */
    void completeMove(const std::string& source, const std::string& destination, bool isDir, uint64_t size = 0);

    /**
     * @brief Retrurns the next file transfer executor index
     * @return unsigned
     */
    unsigned nextExecutorIndex();

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
     * @brief Executors for async file transfers
     */
    std::vector<std::unique_ptr<Executor>> m_transferExecutors;

    /**
     * @brief Index of the last used transfer executor
     */
    unsigned m_lastUsedExecutorIndex;

    /**
     * @brief Number of transfer executors
     */
    unsigned m_numberOfTransferExecutors;

    /**
     * @brief Operations mutex
     */
    mutable std::mutex m_mutex;

    /**
     * @brief Condition variable for waiting for transfers to complete
     */
    std::condition_variable m_transfersCompletedCondVar;

    /**
     * @brief Cipher factory instance
     */
    std::unique_ptr<CipherFactory> m_cipherFactory;

    /**
     * @brief Number of cipher executors
     */
    unsigned m_numberOfCipherExecutors;

    /**
     * @brief Key manager instance
     */
    std::shared_ptr<KeyManager> m_keyManager;

    /**
     * @brief For now only support one type of keys
     */
    static const size_t s_kAcceptableKeyLength;
};

#endif  // FILESERVICEIMPL_H
