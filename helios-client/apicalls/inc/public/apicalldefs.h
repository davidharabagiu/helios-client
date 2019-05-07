#ifndef APICALLDEFS_H
#define APICALLDEFS_H

#include <functional>
#include <string>
#include <vector>
#include <cstdint>
#include <tuple>

/**
 * @brief Status codes for service API calls
 */
enum class ApiCallStatus
{
    SUCCESS,
    USERNAME_ALREADY_EXISTS,
    INVALID_USERNAME,
    INVALID_PASSWORD,
    INVALID_CREDENTIALS,
    UNAUTHORIZED,
    FILE_ALREADY_EXISTS,
    INVALID_PATH,
    INVALID_TRANSFER_ID,
    INVALID_REPLY_FORMAT,
    UNKNOWN_ERROR
};

namespace ApiCallbacks
{
/**
 * @brief Register user callback function. Receives the call status.
 */
using RegisterUserCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Login callback function. Receives the call status and the authentication token.
 */
using LoginCallback = std::function<void(ApiCallStatus, const std::string&)>;

/**
 * @brief Logout callback function. Receives the call status.
 */
using LogoutCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Check token callback function. Receives the call status.
 */
using CheckTokenCallback = std::function<void(ApiCallStatus)>;

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
 * @brief List callback function. Receives the call status and a list of files and directories names, along with a
 * boolean indicating if the file is a directory.
 */
using ListCallback = std::function<void(ApiCallStatus, const std::vector<std::tuple<std::string, bool>>&)>;

/**
 * @brief Remove callback function. Receives the call status.
 */
using RemoveCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Move callback function. Receives the call status.
 */
using MoveCallback = std::function<void(ApiCallStatus)>;
}  // namespace ApiCallbacks

#endif  // APICALLDEFS_H
