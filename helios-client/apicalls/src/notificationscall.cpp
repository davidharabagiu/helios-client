#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDebug>
#include <tuple>

#include "notificationscall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"
#include "typeconversions.h"

const std::string NotificationsCall::s_kUrl                = "notifications";
const std::string NotificationsCall::s_kTokenParam         = "token";
const QString     NotificationsCall::s_kIdJsonField        = "id";
const QString     NotificationsCall::s_kTextJsonField      = "text";
const QString     NotificationsCall::s_kTypeJsonField      = "type";
const QString     NotificationsCall::s_kFileShareJsonValue = "fileshare";
const QString     NotificationsCall::s_kKeyShareJsonValue  = "keyshare";

NotificationsCall::NotificationsCall(const std::string& authToken, const ApiCallbacks::NotificationsCallback& callback)
    : m_callback(callback)
{
    auto requestFactory = Single<DependencyInjector>::instance().getInstance<HttpRequestFactory>();
    if (!requestFactory)
    {
        qFatal("HttpRequestFactory instance not available");
    }

    auto config = Single<DependencyInjector>::instance().getInstance<Config>();
    if (!config)
    {
        qFatal("Config instance not available");
    }

    m_request = requestFactory->createUrlEncodedRequest(
        config->get(ConfigKeys::kServerUrlKey).toString().toStdString() + "/" + s_kUrl);
    m_request->setHeaderValue(s_kTokenParam, authToken);
}

std::shared_ptr<UrlEncodedRequest> NotificationsCall::request()
{
    return m_request;
}

void NotificationsCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void NotificationsCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        QJsonParseError jsonError;
        auto            json = QJsonDocument::fromJson(
            QByteArray(reinterpret_cast<const char*>(reply.data()), safe_integral_cast<int>(reply.size())), &jsonError);
        if (jsonError.error != QJsonParseError::NoError)
        {
            qCritical() << "JSON parse error: " << jsonError.errorString();
            m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, {});
            return;
        }

        std::vector<std::tuple<std::string, std::string, NotificationType>> notifications;

        auto jsonArray = json.array();
        for (const auto& jsonArrayElement : jsonArray)
        {
            if (!jsonArrayElement.isObject())
            {
                qCritical() << "Invalid json reply received: " << replyStr.c_str();
                m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, {});
            }
            auto jsonObject    = jsonArrayElement.toObject();
            auto jsonIdField   = jsonObject[s_kIdJsonField];
            auto jsonTextField = jsonObject[s_kTextJsonField];
            auto jsonTypeField = jsonObject[s_kTypeJsonField];
            if (jsonIdField.type() != QJsonValue::String || jsonTextField.type() != QJsonValue::String ||
                jsonTypeField.type() != QJsonValue::String ||
                (jsonTypeField.toString() != "fileshare" && jsonTypeField.toString() != "keyshare"))
            {
                qCritical() << "Invalid json reply received: " << replyStr.c_str();
                m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, {});
                return;
            }

            notifications.emplace_back(
                jsonIdField.toString().toStdString(), jsonTextField.toString().toStdString(),
                jsonTypeField.toString() == "fileshare" ? NotificationType::FILE_SHARE : NotificationType::KEY_SHARE);
        }

        m_callback(ApiCallStatus::SUCCESS, notifications);
        return;
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, {});
        return;
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, {});
}
