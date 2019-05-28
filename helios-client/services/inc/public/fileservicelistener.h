#ifndef FILESERVICELISTENER_H
#define FILESERVICELISTENER_H

#include <string>
#include <cstdint>

// Forward declarations
class File;
class FileTransfer;

/**
 * @class FileServiceListener
 * @brief Interface for listening on FileService async operations
 */
class FileServiceListener
{
public:
    /**
     * @brief Destructor
     */
    virtual ~FileServiceListener() = default;

    /**
     * @brief The current directory path has changed.
     */
    virtual void currentDirectoryChanged() = 0;

    /**
     * @brief A directory was created
     * @param directory - Created directory
     */
    virtual void directoryCreated(std::shared_ptr<const File> directory) = 0;

    /**
     * @brief A file or directory was moved
     * @param oldFile - Old file. Contains only information about name and path.
     * @param file - File or directory that was moved
     */
    virtual void fileMoved(std::shared_ptr<const File> oldFile, std::shared_ptr<const File> file) = 0;

    /**
     * @brief A file or directory was removed
     * @param directory - Directory which was removed
     */
    virtual void fileRemoved(std::shared_ptr<const File> directory) = 0;

    /**
     * @brief A file was uploaded in the current directory
     * @note transferCompleted or transferAborted will also be called
     * @param file - File
     */
    virtual void uploadedFileInCurrentDir(std::shared_ptr<const File> file) = 0;

    /**
     * @brief A file transfer has started
     * @param transfer - File transfer
     */
    virtual void transferStarted(std::shared_ptr<FileTransfer> transfer) = 0;

    /**
     * @brief Progress has changed for a file transfer.
     * @param transfer - File transfer
     */
    virtual void transferProgressChanged(std::shared_ptr<FileTransfer> transfer) = 0;

    /**
     * @brief A file transfer has completed.
     * @param transfer - File transfer
     */
    virtual void transferCompleted(std::shared_ptr<FileTransfer> transfer) = 0;

    /**
     * @brief A file transfer was aborted by the user or due to an error. If it was aborted due to an error, an
     * errorOccured callback will follow with the reason.
     * @param transfer - File traansfer
     */
    virtual void transferAborted(std::shared_ptr<FileTransfer> transfer) = 0;

    /**
     * @brief A file was shared successfully
     */
    virtual void fileShared() = 0;

    /**
     * @brief A shared file from another user was accepted
     * @param file - New file
     */
    virtual void acceptedFileShare(std::shared_ptr<const File> file) = 0;

    /**
     * @brief An operation has failed.
     * @param errorString - Error message
     */
    virtual void errorOccured(const std::string& errorString) = 0;
};

#endif  // FILESERVICELISTENER_H
