#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

#include "defaultsettingsproviderimpl.h"
#include "paths.h"

DefaultSettingsProviderImpl::DefaultSettingsProviderImpl()
{
    QFile defaultSettingsFile(QString::fromStdString(Paths::kDefaultSettingsFile));
    if (!defaultSettingsFile.exists())
    {
        qFatal("Default settings file %s not found", Paths::kDefaultSettingsFile.c_str());
    }

    defaultSettingsFile.open(QIODevice::ReadOnly);

    QJsonParseError err;
    auto            json = QJsonDocument::fromJson(defaultSettingsFile.readAll(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qFatal("Error while processing %s: %s", Paths::kDefaultSettingsFile.c_str(),
               err.errorString().toStdString().c_str());
    }

    defaultSettingsFile.close();

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
            qFatal("Unknown json value type for default setting %s: %d", it.key().toStdString().c_str(),
                   static_cast<int>(it.value().type()));
        }

        m_valuesRegistry.emplace(it.key().toStdString(), setting);
    }
}

QVariant DefaultSettingsProviderImpl::get(const std::string& key) const
{
    auto it = m_valuesRegistry.find(key);
    if (it == m_valuesRegistry.end())
    {
        return QVariant();
    }
    return it->second;
}
