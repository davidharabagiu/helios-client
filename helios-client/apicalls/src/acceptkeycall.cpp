#include <QDebug>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "acceptkeycall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"
#include "typeconversions.h"

const std::string AcceptKeyCall::s_kUrl                          = "acceptkey";
const std::string AcceptKeyCall::s_kTokenParam                   = "token";
const std::string AcceptKeyCall::s_kNotificationIdParam          = "notification";
const QString     AcceptKeyCall::s_kKeyNameJsonField             = "name";
const QString     AcceptKeyCall::s_kKeyLengthJsonField           = "length";
const QString     AcceptKeyCall::s_kKeyContentJsonField          = "content";
const std::string AcceptKeyCall::s_kErrorInvalidNotificationType = "Invalid notification type";

AcceptKeyCall::AcceptKeyCall(const std::string& authToken, const std::string& notificationId,
                             const ApiCallbacks::AcceptKeyCallback& callback)
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
    m_request->setParameter(s_kNotificationIdParam, notificationId);
}

std::shared_ptr<UrlEncodedRequest> AcceptKeyCall::request()
{
    return m_request;
}

void AcceptKeyCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void AcceptKeyCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
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
            m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, "", 0, {});
            return;
        }

        if (!json.isObject())
        {
            qCritical() << "Invalid json reply received: " << replyStr.c_str();
            m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, "", 0, {});
        }
        auto jsonKeyNameField    = json[s_kKeyNameJsonField];
        auto jsonKeyLengthField  = json[s_kKeyLengthJsonField];
        auto jsonKeyContentField = json[s_kKeyContentJsonField];
        if (jsonKeyNameField.type() != QJsonValue::String || jsonKeyLengthField.type() != QJsonValue::Double ||
            jsonKeyContentField.type() != QJsonValue::String)
        {
            qCritical() << "Invalid json reply received: " << replyStr.c_str();
            m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, "", 0, {});
            return;
        }

        QByteArray key = QByteArray::fromBase64(jsonKeyContentField.toString().toUtf8());
        m_callback(ApiCallStatus::SUCCESS, jsonKeyNameField.toString().toStdString(),
                   safe_integral_cast<uint16_t>(jsonKeyLengthField.toInt()),
                   std::vector<uint8_t>(key.begin(), key.end()));
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, "", 0, {});
    }
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorInvalidNotificationType)
    {
        m_callback(ApiCallStatus::INVALID_NOTIFICATION_TYPE, "", 0, {});
    }
    else
    {
        qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                    << replyStr.c_str();
        m_callback(ApiCallStatus::UNKNOWN_ERROR, "", 0, {});
    }
}
