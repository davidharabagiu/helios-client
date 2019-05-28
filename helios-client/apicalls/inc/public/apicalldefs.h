#ifndef APICALLDEFS_H
#define APICALLDEFS_H

#include <functional>
#include <string>
#include <vector>
#include <cstdint>
#include <tuple>
#include <optional>

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
    NO_KEY_SPECIFIED,
    INVALID_NOTIFICATION_TYPE,
    FILE_NO_LONGER_EXISTS,
    UNKNOWN_ERROR
};

enum class NotificationType
{
    FILE_SHARE,
    KEY_SHARE
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
 * @brief Get user key callback function. Receives the call status and the key in base64 format.
 */
using GetUserKeyCallback = std::function<void(ApiCallStatus, const std::string&)>;

/**
 * @brief Set user key callback function. Receives the call status.
 */
using SetUserKeyCallback = std::function<void(ApiCallStatus)>;

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
 * @brief Begin download callback function. Receives the call status, the id of the transfer and the size of the file.
 */
using BeginDownloadCallback = std::function<void(ApiCallStatus, const std::string&, uint64_t)>;

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
 * boolean indicating if the file is a directory and the size of the file if it's not a directory.
 */
using ListCallback =
    std::function<void(ApiCallStatus, const std::vector<std::tuple<std::string, bool, std::optional<uint64_t>>>&)>;

/**
 * @brief Remove callback function. Receives the call status.
 */
using RemoveCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Share key callback function. Receives the call status.
 */
using ShareKeyCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Accept shared key callback function. Receives the call status, key name, key length and key content.
 */
using AcceptKeyCallback = std::function<void(ApiCallStatus, const std::string& keyName, uint16_t keyLength,
                                             const std::vector<uint8_t>& keyContent)>;

/**
 * @brief Share file callback function. Receives the call status.
 */
using ShareCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Accept file callback function. Receives the call status.
 */
using AcceptFileCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Move callback function. Receives the call status.
 */
using MoveCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Is directory callback function. Receives the call status and if the provided path reffers to a directory.
 */
using IsDirCallback = std::function<void(ApiCallStatus, bool)>;

/**
 * @brief Notifications callback function. Receives a list of notifications. Each notification is a tuple containing the
 * notification id, notification text and notification type.
 */
using NotificationsCallback =
    std::function<void(ApiCallStatus, const std::vector<std::tuple<std::string, std::string, NotificationType>>&)>;

/**
 * @brief Dismiss notification callback function. Receives the call status.
 */
using DismissNotificationCallback = std::function<void(ApiCallStatus)>;

/**
 * @brief Dismiss all notification callback function. Receives the call status.
 */
using DismissAllNotificationsCallback = std::function<void(ApiCallStatus)>;
}  // namespace ApiCallbacks

#endif  // APICALLDEFS_H
