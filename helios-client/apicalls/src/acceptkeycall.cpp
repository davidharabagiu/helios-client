#include <QDebug>
#include <QByteArray>

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
        QByteArray key = QByteArray::fromBase64(QByteArray::fromStdString(replyStr));
        m_callback(ApiCallStatus::SUCCESS, std::vector<uint8_t>(key.begin(), key.end()));
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, {});
    }
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorInvalidNotificationType)
    {
        m_callback(ApiCallStatus::INVALID_NOTIFICATION_TYPE, {});
    }
    else
    {
        qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                    << replyStr.c_str();
        m_callback(ApiCallStatus::UNKNOWN_ERROR, {});
    }
}
