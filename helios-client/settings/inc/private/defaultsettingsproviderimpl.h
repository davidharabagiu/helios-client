#ifndef DEFAULTSETTINGSPROVIDERIMPL_H
#define DEFAULTSETTINGSPROVIDERIMPL_H

#include "defaultsettingsprovider.h"

/**
 * @class DefaultSettingsProviderImpl
 * @brief Private implementation of DefaultSettingsProvider
 */
class DefaultSettingsProviderImpl : public DefaultSettingsProvider
{
public:
    /**
     * @brief Constructor
     */
    DefaultSettingsProviderImpl();

public:  // from DefaultSettingsProvider
    QVariant get(const std::string& key) const;

private:
    /**
     * @brief Registry which holds all the default settings values indexed by their keys
     */
    std::map<std::string, QVariant> m_valuesRegistry;
};

#endif  // DEFAULTSETTINGSPROVIDERIMPL_H
