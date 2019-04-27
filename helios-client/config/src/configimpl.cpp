#include <QFile>
#include <QJsonDocument>
#include <QDebug>

#include "configimpl.h"
#include "configkeys.h"
#include "paths.h"

ConfigImpl::ConfigImpl()
{
    QFile configFile(Paths::kConfigFile);
    if (!configFile.exists())
    {
        qFatal("Configuration file " + Paths::kConfigFile + " not found.");
    }

    QJsonParseError err;
    auto            json = QJsonDocument::fromJson(configFile.readAll(), &err);
    if (err.error != QJsonParseError::ParseError::NoError)
    {
        qFatal("Error while processing " + Paths::kConfigFile + ": " + err.errorString().toStdString());
    }

    auto serverUrlObj = json[ConfigKeys::kServerUrlKey];
    if (serverUrlObj.type() != QJsonValue::String)
    {
        qFatal(ConfigKeys::kServerUrlKey + " not found in " + Paths::kConfigFile);
    }

    m_valuesRegistry.emplace(ConfigKeys::kServerUrlKey, serverUrlObj.toString().toStdString());
}

std::any ConfigImpl::get(const std::string& key) const
{
    return m_valuesRegistry.at(key);
}
