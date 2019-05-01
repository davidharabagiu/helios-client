#include <QFile>
#include <QJsonDocument>
#include <QDebug>

#include "configimpl.h"
#include "configkeys.h"
#include "paths.h"

ConfigImpl::ConfigImpl()
{
    QFile configFile(QString::fromStdString(Paths::kConfigFile));
    if (!configFile.exists())
    {
        qFatal("Configuration file %s not found", Paths::kConfigFile.c_str());
    }

    configFile.open(QIODevice::ReadOnly);

    QJsonParseError err;
    auto            json = QJsonDocument::fromJson(configFile.readAll(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qFatal("Error while processing %s: %s", Paths::kConfigFile.c_str(), err.errorString().toStdString().c_str());
    }

    configFile.close();

    auto serverUrlObj = json[QString::fromStdString(ConfigKeys::kServerUrlKey)];
    if (serverUrlObj.type() != QJsonValue::String)
    {
        qFatal("%s not found in %s", ConfigKeys::kServerUrlKey.c_str(), Paths::kConfigFile.c_str());
    }

    m_valuesRegistry.emplace(ConfigKeys::kServerUrlKey, serverUrlObj.toString());
}

QVariant ConfigImpl::operator[](const std::string& key) const
{
    return get(key);
}

QVariant ConfigImpl::get(const std::string& key) const
{
    auto it = m_valuesRegistry.find(key);
    if (it == m_valuesRegistry.end())
    {
        return QVariant();
    }
    else
    {
        return it->second;
    }
}
