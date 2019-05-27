#ifndef PATHS_H
#define PATHS_H

#include <string>

namespace Paths
{
const std::string kSeparator              = "/";
const std::string kQmlPath                = "qml";
const std::string kMainQmlFile            = "main.qml";
const std::string kConfigFile             = "config.json";
const std::string kDefaultSettingsFile    = "default_settings.json";
const std::string kSettingsFile           = "settings.json";
const std::string kKeyStorageFile         = "keys";
const std::string kTemporaryPublicKeyFile = ".temp.pubk";
}  // namespace Paths

#endif  // PATHS_H
