#include <QDebug>
#include <QFile>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>

#include "settingsmanagerimpl.h"
#include "defaultsettingsprovider.h"
#include "paths.h"

SettingsManagerImpl::SettingsManagerImpl(std::unique_ptr<DefaultSettingsProvider> defaultSettingsProvider)
    : m_defaultSettingsProvider(std::move(defaultSettingsProvider))
{
    QFile settingsFile(QString::fromStdString(Paths::kSettingsFile));
    if (!settingsFile.exists())
    {
        qInfo() << "Settings file " << Paths::kSettingsFile.c_str() << " not found";
        return;
    }

    settingsFile.open(QIODevice::ReadOnly);

    QJsonParseError err;
    auto            json = QJsonDocument::fromJson(settingsFile.readAll(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qCritical() << "Error while processing " << Paths::kSettingsFile.c_str() << ": "
                    << err.errorString().toStdString().c_str();
    }

    settingsFile.close();

    const auto& jsonObj = json.object();
    for (auto it = jsonObj.constBegin(); it != jsonObj.constEnd(); ++it)
    {
        QVariant setting;

        if (it.value().isBool())
        {
            setting = QVariant(it.value().toBool());
        }
        else if (it.value().isDouble())
        {
            setting = QVariant(it.value().toDouble());
        }
        else if (it.value().isString())
        {
            setting = QVariant(it.value().toString());
        }
        else
        {
            qCritical() << "Unknown json value type for default setting " << it.key() << ": "
                        << static_cast<int>(it.value().type());
        }

        if (setting.isValid())
        {
            m_valuesRegistry.emplace(it.key().toStdString(), setting);
        }
    }
}

SettingsManagerImpl::~SettingsManagerImpl()
{
    save();
}

QVariant SettingsManagerImpl::get(const std::string& key) const
{
    auto it = m_valuesRegistry.find(key);
    if (it == m_valuesRegistry.end())
    {
        return m_defaultSettingsProvider->get(key);
    }
    return it->second;
}

void SettingsManagerImpl::set(const std::string& key, bool value)
{
    m_valuesRegistry[key] = value;
    Observable::notifyAll(&SettingsListener::settingChanged, key);
}

void SettingsManagerImpl::set(const std::string& key, double value)
{
    m_valuesRegistry[key] = value;
    Observable::notifyAll(&SettingsListener::settingChanged, key);
}

void SettingsManagerImpl::set(const std::string& key, const QString& value)
{
    m_valuesRegistry[key] = value;
    Observable::notifyAll(&SettingsListener::settingChanged, key);
}

void SettingsManagerImpl::reset(const std::string& key)
{
    m_valuesRegistry.erase(key);
    Observable::notifyAll(&SettingsListener::settingChanged, key);
}

void SettingsManagerImpl::reset()
{
    // collect all keys
    std::vector<std::string> keys;
    keys.reserve(m_valuesRegistry.size());
    for (const auto& el : m_valuesRegistry)
    {
        keys.push_back(el.first);
    }

    m_valuesRegistry.clear();

    for (const auto& key : keys)
    {
        Observable::notifyAll(&SettingsListener::settingChanged, key);
    }
}

void SettingsManagerImpl::save() const
{
    if (m_valuesRegistry.empty())
    {
        QFile(QString::fromStdString(Paths::kSettingsFile)).remove();
    }

    QJsonObject jsonObj;
    for (const auto& el : m_valuesRegistry)
    {
        QJsonValue jsonVal;
        if (el.second.type() == QVariant::Bool)
        {
            jsonVal = el.second.toBool();
        }
        else if (el.second.type() == QVariant::Double)
        {
            jsonVal = el.second.toDouble();
        }
        else if (el.second.type() == QVariant::String)
        {
            jsonVal = el.second.toString();
        }
        jsonObj.insert(el.first.c_str(), jsonVal);
    }
    QJsonDocument jsonDoc(jsonObj);

    QFile settingsFile(QString::fromStdString(Paths::kSettingsFile));
    settingsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    settingsFile.write(jsonDoc.toJson());
    settingsFile.close();
}
