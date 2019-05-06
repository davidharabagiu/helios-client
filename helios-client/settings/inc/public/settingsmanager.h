#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QVariant>
#include <QString>
#include <string>

#include "observable.h"
#include "settingslistener.h"

/**
 * @class SettingsManager
 * @brief Interface for application settings management
 */
class SettingsManager : public Observable<SettingsListener, ObservableNotifyMode::ASYNC>
{
public:
    /**
     * @brief Destructor
     */
    virtual ~SettingsManager() = default;

    /**
     * @brief Returns a setting value. If the value with the given key could not be found, it returns an empty
     * object.
     * @param key - Setting key
     * @return QVariant
     */
    virtual QVariant get(const std::string& key) const = 0;

    /**
     * @brief Set a boolean setting value with the provided key.
     * @param key - Setting key
     * @param value - Setting balue
     */
    virtual void set(const std::string& key, bool value) = 0;

    /**
     * @brief Set a double setting value with the provided key.
     * @param key - Setting key
     * @param value - Setting balue
     */
    virtual void set(const std::string& key, double value) = 0;

    /**
     * @brief Set a string setting value with the provided key.
     * @param key - Setting key
     * @param value - Setting balue
     */
    virtual void set(const std::string& key, const QString& value) = 0;

    /**
     * @brief Reset a setting. If the setting with the given key is a default setting, it will be reset to the default
     * value. If the setting is user provided, it will be removed.
     * @param key - Setting key
     */
    virtual void reset(const std::string& key) = 0;

    /**
     * @brief Reset all setting. If a setting is a default setting, it will be reset to the default value. If the
     * setting is user provided, it will be removed.
     */
    virtual void reset() = 0;

    /**
     * @brief Save all settings modifications on persistent storage. This shold be called regularly, prefferably after
     * every set or reset, in order to prevent data loss.
     */
    virtual void save() const = 0;
};

#endif  // SETTINGSMANAGER_H
