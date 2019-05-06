#ifndef SETTINGSLISTENER_H
#define SETTINGSLISTENER_H

#include <string>

/**
 * @class SettingsListener
 * @brief Defined an interface for observing settings changes
 */
class SettingsListener
{
public:
    /**
     * @brief Destructor
     */
    virtual ~SettingsListener() = default;

    /**
     * @brief Called when a setting has changed its value
     * @param key - Setting key
     */
    virtual void settingChanged(const std::string& key) = 0;
};

#endif  // SETTINGSLISTENER_H
