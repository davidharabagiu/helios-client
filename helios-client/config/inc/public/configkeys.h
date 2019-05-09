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
 * @brief Settings auto-save interval in milliseconds (0 = disable auto-save)
 * @note Type = uint
 */
const std::string kSettingsAutoSaveIntervalKey = "settingsAutoSaveInterval";

/**
 * @brief File upload chunk size in KB
 * @note Type = uint
 */
const std::string kUploadChunkSize = "uploadChunkSize";
}  // namespace ConfigKeys

#endif  // CONFIGKEYS_H
