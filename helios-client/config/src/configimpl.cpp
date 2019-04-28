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

    QJsonParseError err;
    auto            json = QJsonDocument::fromJson(configFile.readAll(), &err);
    if (err.error != QJsonParseError::ParseError::NoError)
    {
        qFatal("Error while processing %s: %s", Paths::kConfigFile.c_str(), err.errorString().toStdString().c_str());
    }

    auto serverUrlObj = json[QString::fromStdString(ConfigKeys::kServerUrlKey)];
    if (serverUrlObj.type() != QJsonValue::String)
    {
        qFatal("%s not found in %s", ConfigKeys::kServerUrlKey.c_str(), Paths::kConfigFile.c_str());
    }

    m_valuesRegistry.emplace(ConfigKeys::kServerUrlKey, serverUrlObj.toString());
}

QVariant ConfigImpl::get(const std::string& key) const
{
    return m_valuesRegistry.at(key);
}
