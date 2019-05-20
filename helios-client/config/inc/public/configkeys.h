#ifndef CONFIGKEYS_H
#define CONFIGKEYS_H

#include <string>

namespace ConfigKeys
{
/**
 * @brief Server url config key
 * @note Type = QString
 */
const std::string kServerUrlKey = "serverUrl";

/**
 * @brief Ignore SSL errors (e.g. Untrusted certificate source). You should only set this value to true in a
 * non-production environment, with a development server.
 * @note Type = bool
 */
const std::string kIgnoreSslErrors = "ignoreSslErrors";

/**
 * @brief Settings auto-save interval in milliseconds (0 = disable auto-save)
 * @note Type = uint
 */
const std::string kSettingsAutoSaveIntervalKey = "settingsAutoSaveInterval";

/**
 * @brief File upload chunk size in KB
 * @note Type = uint
 */
const std::string kUploadChunkSize = "uploadChunkSize";

/**
 * @brief Number of file transfers that can run in parallel
 * @note Type = uint
 */
const std::string kNumberOfTransferExecutors = "numberOfTransferExecutors";

/**
 * @brief Number of file encryption / decryption jobs that can run in parallel
 * @note Type = uint
 */
const std::string kNumberOfCipherExecutors = "numberOfCipherExecutors";
}  // namespace ConfigKeys

#endif  // CONFIGKEYS_H
