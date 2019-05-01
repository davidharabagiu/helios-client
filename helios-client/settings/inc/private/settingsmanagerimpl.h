#ifndef SETTINGSMANAGERIMPL_H
#define SETTINGSMANAGERIMPL_H

#include <map>
#include <memory>

#include "settingsmanager.h"

class DefaultSettingsProvider;

/**
 * @class SettingsManagerImpl
 * @brief Private implementation of SettingsManager
 */
class SettingsManagerImpl : public SettingsManager
{
public:
    /**
     * @brief Constructor
     */
    SettingsManagerImpl();

public:  // from SettingsManager
    QVariant get(const std::string& key) const override;
    void     set(const std::string& key, bool value) override;
    void     set(const std::string& key, double value) override;
    void     set(const std::string& key, const QString& value) override;
    void     reset(const std::string& key) override;
    void     reset() override;
    void     save() const override;

private:
    /**
     * @brief Registry which holds the user defined settings values and the default settings values which were modified
     * by the user, indexed by their keys
     */
    std::map<std::string, QVariant> m_valuesRegistry;

    /**
     * @brief Provider for default settings values
     */
    std::shared_ptr<DefaultSettingsProvider> m_defaultSettingsProvider;
};

#endif  // SETTINGSMANAGERIMPL_H
