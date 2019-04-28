#ifndef FILEAPICALLER_H
#define FILEAPICALLER_H

#include <string>
#include <functional>
#include <cstdint>
#include <vector>

#include "apicalldefs.h"

/**
 * @class FileApiCaller
 * @brief Provides calls for the file management API
 */
class FileApiCaller
{
public:
    /**
     * @brief Destructor
     */
    virtual ~FileApiCaller() = default;

    /**
     * @brief Create a new directory in the user's storage.
     * @param authToken - User authentication token
     * @param path - Path of the new directory
     * @param callback - CreateDirectoryCallback
     */
    virtual void createDirectory(const std::string& authToken, const std::string& path,
                                 const ApiCallbacks::CreateDirectoryCallback& callback) const = 0;

    /**
     * @brief Begin a file upload operation. The callback will contain a transfer id used to upload the file contents
     * using the upload call. If the file doesn't exist, then it will be created. If it exists, the uploaded content
     * will replace the existing one.
     * @param authToken - User authentication token
     * @param path - Path of the uploaded file
     * @param callback - BeginUploadCallback
     */
    virtual void beginUpload(const std::string& authToken, const std::string& path,
                             const ApiCallbacks::BeginUploadCallback& callback) const = 0;

    /**
     * @brief Upload a chunk of a file in the user's storage.
     * @param authToken - User authentication token
     * @param transferId - Transfer id retrieved using the beginUpload call
     * @param offset - Offset in the file where to insert the uploaded data chunk
     * @param content - File data chunk
     * @param callback - UploadCallback
     */
    virtual void upload(const std::string& authToken, const std::string& transferId, uint64_t offset,
                        const std::vector<uint8_t>& content, const ApiCallbacks::UploadCallback& callback) const = 0;

    /**
     * @brief Begin a file download operation. The callback will contain a transfer id used to download the file
     * contents using the download call.
     * @param authToken - User authentication token
     * @param path - Path of the file to download
     * @param callback - BeginDownloadCallback
     */
    virtual void beginDownload(const std::string& authToken, const std::string& path,
                               const ApiCallbacks::BeginDownloadCallback& callback) const = 0;

    /**
     * @brief Download a chunk of a file from the user's storage.
     * @param authToken - User authentication token
     * @param transferId - Transfer id retrieved using the beginDownload call
     * @param offset - Offset in the file from where to retrieve the file contents
     * @param size - Retrieved file chunk size
     * @param callback - DownloadCallback
     */
    virtual void download(const std::string& authToken, const std::string& transferId, uint64_t offset, uint64_t size,
                          const ApiCallbacks::DownloadCallback& callback) const = 0;

    /**
     * @brief Ends a file upload or download operation.
     * @param authToken - User authentication token
     * @param transferId - Transfer id retrieved using the beginUpload or beginDownload call
     * @param callback - EndTransferCallback
     */
    virtual void endTransfer(const std::string& authToken, const std::string& transferId,
                             const ApiCallbacks::EndTransferCallback& callback) const = 0;

    /**
     * @brief Retrieve size information about a file from the user's storage.
     * @param authToken - User authentication token
     * @param path - File path
     * @param callback - GetFileSizeCallback
     */
    virtual void getFileSize(const std::string& authToken, const std::string& path,
                             const ApiCallbacks::GetFileSizeCallback& callback) const = 0;

    /**
     * @brief List all the files and subdirectories of a directory from the user's storage.
     * @param authToken - User authentication token
     * @param path - Directory path
     * @param callback - ListCallback
     */
    virtual void list(const std::string& authToken, const std::string& path,
                      const ApiCallbacks::ListCallback& callback) const = 0;

    /**
     * @brief Delete a file or directory from the user's storage. If the path reffers to a directory, then the directory
     * will be deleted recursively.
     * @param authToken - User authentication token
     * @param path - File or directory path
     * @param callback - RemoveCallback
     */
    virtual void remove(const std::string& authToken, const std::string& path,
                        const ApiCallbacks::RemoveCallback& callback) const = 0;

    /**
     * @brief Move a file or directory to another location in the user's storage.
     * @param authToken - User authentication token
     * @param source - File or directory path
     * @param destination - Destination path
     * @param callback - MoveCallback
     */
    virtual void move(const std::string& authToken, const std::string& source, const std::string& destination,
                      const ApiCallbacks::MoveCallback& callback) const = 0;
};

#endif  // FILEAPICALLER_H
