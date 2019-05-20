#include <QFile>
#include <QJsonDocument>
#include <QDebug>

#include "configimpl.h"
#include "configkeys.h"
#include "paths.h"
#include "typeconversions.h"

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

    auto ignoreSslErrorsObj = json[QString::fromStdString(ConfigKeys::kIgnoreSslErrors)];
    if (ignoreSslErrorsObj.type() != QJsonValue::Bool)
    {
        qFatal("%s not found in %s", ConfigKeys::kIgnoreSslErrors.c_str(), Paths::kConfigFile.c_str());
    }
    m_valuesRegistry.emplace(ConfigKeys::kIgnoreSslErrors, ignoreSslErrorsObj.toBool());

    auto settingsAutoSaveIntervalObj = json[QString::fromStdString(ConfigKeys::kSettingsAutoSaveIntervalKey)];
    if (settingsAutoSaveIntervalObj.type() != QJsonValue::Double)
    {
        qFatal("%s not found in %s", ConfigKeys::kSettingsAutoSaveIntervalKey.c_str(), Paths::kConfigFile.c_str());
    }
    m_valuesRegistry.emplace(ConfigKeys::kSettingsAutoSaveIntervalKey,
                             safe_integral_cast<uint>(settingsAutoSaveIntervalObj.toInt()));

    auto uploadChunkSizeObj = json[QString::fromStdString(ConfigKeys::kUploadChunkSize)];
    if (uploadChunkSizeObj.type() != QJsonValue::Double)
    {
        qFatal("%s not found in %s", ConfigKeys::kUploadChunkSize.c_str(), Paths::kConfigFile.c_str());
    }
    m_valuesRegistry.emplace(ConfigKeys::kUploadChunkSize, safe_integral_cast<uint>(uploadChunkSizeObj.toInt()));

    auto numberOfTransferExecutorsObj = json[QString::fromStdString(ConfigKeys::kNumberOfTransferExecutors)];
    if (numberOfTransferExecutorsObj.type() != QJsonValue::Double)
    {
        qFatal("%s not found in %s", ConfigKeys::kNumberOfTransferExecutors.c_str(), Paths::kConfigFile.c_str());
    }
    m_valuesRegistry.emplace(ConfigKeys::kNumberOfTransferExecutors,
                             safe_integral_cast<uint>(numberOfTransferExecutorsObj.toInt()));

    auto numberOfCipherExecutorsObj = json[QString::fromStdString(ConfigKeys::kNumberOfCipherExecutors)];
    if (numberOfCipherExecutorsObj.type() != QJsonValue::Double)
    {
        qFatal("%s not found in %s", ConfigKeys::kNumberOfCipherExecutors.c_str(), Paths::kConfigFile.c_str());
    }
    m_valuesRegistry.emplace(ConfigKeys::kNumberOfCipherExecutors,
                             safe_integral_cast<uint>(numberOfCipherExecutorsObj.toInt()));
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
