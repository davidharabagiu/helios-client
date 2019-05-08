#ifndef FILESERVICELISTENER_H
#define FILESERVICELISTENER_H

#include <string>
#include <cstdint>

/**
 * @class FileServiceListener
 * @brief Interface for listening on FileService async operations
 */
class FileServiceListener
{
public:
    /**
     * @brief The current directory path has changed.
     */
    virtual void currentDirectoryChanged() = 0;

    /**
     * @brief A directory was created
     * @param path - Directory path
     */
    virtual void directoryCreated(const std::string& path) = 0;

    /**
     * @brief A file or directory was moved
     * @param sourcePath - Source path
     * @param destinationPath - Destination path
     */
    virtual void fileMoved(const std::string& sourcePath, const std::string& destinationPath) = 0;

    /**
     * @brief A file or directory was removed
     * @param path - Path
     */
    virtual void fileRemoved(const std::string& path) = 0;

    /**
     * @brief A file download operation has started
     * @param path - File path
     */
    virtual void fileDownloadStarted(const std::string& path) = 0;

    /**
     * @brief A file upload operation has started
     * @param path - File path
     */
    virtual void fileUploadStarted(const std::string& path) = 0;

    /**
     * @brief Progress has changed for a file download or upload operation.
     * @param path - File path
     */
    virtual void fileOperationProgressChanged(const std::string& path) = 0;

    /**
     * @brief A file download or upload operation has completed.
     * @param path - File path
     */
    virtual void fileOperationCompleted(const std::string& path) = 0;

    /**
     * @brief A file download or upload operation was aborted by the user or due to an error. If it was aborted due to
     * an error, an errorOccured callback will follow with the reason.
     * @param path - File path
     */
    virtual void fileOperationAborted(const std::string& path) = 0;

    /**
     * @brief An operation has failed.
     * @param errorString - Error message
     */
    virtual void errorOccured(const std::string& errorString) = 0;
};

#endif  // FILESERVICELISTENER_H
