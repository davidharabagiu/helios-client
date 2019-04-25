#ifndef FILEAPICALLER_H
#define FILEAPICALLER_H

#include <string>
#include <functional>
#include <cstdint>
#include <vector>

#include "apicallstatus.h"

/**
 * @class FileApiCaller
 * @brief Provides calls for the file management API
 */
class FileApiCaller
{
public:  // callback definitions
    /**
     * @brief Create directory callback function. Receives the call status.
     */
    using CreateDirectoryCallback = std::function<void(ApiCallStatus)>;

    /**
     * @brief Begin upload callback function. Receives the call status and the id of the transfer.
     */
    using BeginUploadCallback = std::function<void(ApiCallStatus, const std::string&)>;

    /**
     * @brief Upload callback function. Receives the call status.
     */
    using UploadCallback = std::function<void(ApiCallStatus)>;

    /**
     * @brief Begin download callback function. Receives the call status and the id of the transfer.
     */
    using BeginDownloadCallback = std::function<void(ApiCallStatus, const std::string&)>;

    /**
     * @brief Download callback function. Receives the call status and the requested file bytes.
     */
    using DownloadCallback = std::function<void(ApiCallStatus, const std::vector<uint8_t>&)>;

    /**
     * @brief End transfer callback function. Receives the call status.
     */
    using EndTransferCallback = std::function<void(ApiCallStatus)>;

    /**
     * @brief Get file size callback function. Receives the call status and the requested file size.
     */
    using GetFileSizeCallback = std::function<void(ApiCallStatus, uint64_t)>;

    /**
     * @brief List callback function. Receives the call status and a list of files and directories names.
     */
    using ListCallback = std::function<void(ApiCallStatus, const std::vector<std::string>&)>;

    /**
     * @brief Remove callback function. Receives the call status.
     */
    using RemoveCallback = std::function<void(ApiCallStatus)>;

    /**
     * @brief Move callback function. Receives the call status.
     */
    using MoveCallback = std::function<void(ApiCallStatus)>;

public:
    /**
     * @brief Create a new directory in the user's storage.
     * @param authToken - User authentication token
     * @param path - Path of the new directory
     * @param callback - CreateDirectoryCallback
     */
    void createDirectory(const std::string& authToken, const std::string& path,
                         const CreateDirectoryCallback& callback);

    /**
     * @brief Begin a file upload operation. The callback will contain a transfer id used to upload the file contents
     * using the upload call. If the file doesn't exist, then it will be created. If it exists, the uploaded content
     * will replace the existing one.
     * @param authToken - User authentication token
     * @param path - Path of the uploaded file
     * @param callback - BeginUploadCallback
     */
    void beginUpload(const std::string& authToken, const std::string& path, const BeginUploadCallback& callback);

    /**
     * @brief Upload a chunk of a file in the user's storage.
     * @param authToken - User authentication token
     * @param transferId - Transfer id retrieved using the beginUpload call
     * @param offset - Offset in the file where to insert the uploaded data chunk
     * @param content - File data chunk
     * @param callback - UploadCallback
     */
    void upload(const std::string& authToken, const std::string& transferId, uint64_t offset,
                const std::vector<uint8_t>& content, const UploadCallback& callback);

    /**
     * @brief Begin a file download operation. The callback will contain a transfer id used to download the file
     * contents using the download call.
     * @param authToken - User authentication token
     * @param path - Path of the file to download
     * @param callback - BeginDownloadCallback
     */
    void beginDownload(const std::string& authToken, const std::string& path, const BeginDownloadCallback& callback);

    /**
     * @brief Download a chunk of a file from the user's storage.
     * @param authToken - User authentication token
     * @param transferId - Transfer id retrieved using the beginDownload call
     * @param offset - Offset in the file from where to retrieve the file contents
     * @param size - Retrieved file chunk size
     * @param callback - DownloadCallback
     */
    void download(const std::string& authToken, const std::string& transferId, uint64_t offset, uint64_t size,
                  const DownloadCallback& callback);

    /**
     * @brief Ends a file upload or download operation.
     * @param authToken - User authentication token
     * @param transferId - Transfer id retrieved using the beginUpload or beginDownload call
     * @param callback - EndTransferCallback
     */
    void endTransfer(const std::string& authToken, const std::string& transferId, const EndTransferCallback& callback);

    /**
     * @brief Retrieve size information about a file from the user's storage.
     * @param authToken - User authentication token
     * @param path - File path
     * @param callback - GetFileSizeCallback
     */
    void getFileSize(const std::string& authToken, const std::string& path, const GetFileSizeCallback& callback);

    /**
     * @brief List all the files and subdirectories of a directory from the user's storage.
     * @param authToken - User authentication token
     * @param path - Directory path
     * @param callback - ListCallback
     */
    void list(const std::string& authToken, const std::string& path, const ListCallback& callback);

    /**
     * @brief Delete a file or directory from the user's storage. If the path reffers to a directory, then the directory
     * will be deleted recursively.
     * @param authToken - User authentication token
     * @param path - File or directory path
     * @param callback - RemoveCallback
     */
    void remove(const std::string& authToken, const std::string& path, const RemoveCallback& callback);

    /**
     * @brief Move a file or directory to another location in the user's storage.
     * @param authToken - User authentication token
     * @param source - File or directory path
     * @param destination - Destination path
     * @param callback - MoveCallback
     */
    void move(const std::string& authToken, const std::string& source, const std::string& destination,
              const MoveCallback& callback);
};

#endif  // FILEAPICALLER_H
