#ifndef DEFAULTSETTINGSPROVIDER_H
#define DEFAULTSETTINGSPROVIDER_H

#include <QVariant>
#include <string>

/**
 * @class DefaultSettingsProvider
 * @brief Provides application default settings
 */
class DefaultSettingsProvider
{
public:
    /**
     * @brief Destructor
     */
    virtual ~DefaultSettingsProvider() = default;

    /**
     * @brief Returns a default setting value. If the value with the given key could not be found, it returns an empty
     * object.
     * @param key - Setting key
     * @return QVariant
     */
    virtual QVariant get(const std::string& key) const = 0;
};

#endif  // DEFAULTSETTINGSPROVIDER_H
