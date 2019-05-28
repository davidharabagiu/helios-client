#include <QDebug>

#include "acceptfilecall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string AcceptFileCall::s_kUrl                          = "acceptfile";
const std::string AcceptFileCall::s_kTokenParam                   = "token";
const std::string AcceptFileCall::s_kNotificationIdParam          = "notification";
const std::string AcceptFileCall::s_kPathParam                    = "path";
const std::string AcceptFileCall::s_kErrorInvalidNotificationType = "Invalid notification type";
const std::string AcceptFileCall::s_kErrorFileNoLongerExists      = "File no longer exists";
const std::string AcceptFileCall::s_kErrorFileAlreadyExists       = "File already exists";
const std::string AcceptFileCall::s_kErrorInvalidPath             = "Invalid path";

AcceptFileCall::AcceptFileCall(const std::string& authToken, const std::string& notificationId, const std::string& path,
                               const ApiCallbacks::AcceptFileCallback& callback)
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
    m_request->setParameter(s_kPathParam, path);
}

std::shared_ptr<UrlEncodedRequest> AcceptFileCall::request()
{
    return m_request;
}

void AcceptFileCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void AcceptFileCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS);
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED);
    }
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorInvalidNotificationType)
    {
        m_callback(ApiCallStatus::INVALID_NOTIFICATION_TYPE);
    }
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorFileNoLongerExists)
    {
        m_callback(ApiCallStatus::FILE_NO_LONGER_EXISTS);
    }
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorFileAlreadyExists)
    {
        m_callback(ApiCallStatus::FILE_ALREADY_EXISTS);
    }
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorInvalidPath)
    {
        m_callback(ApiCallStatus::INVALID_PATH);
    }
    else
    {
        qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                    << replyStr.c_str();
        m_callback(ApiCallStatus::UNKNOWN_ERROR);
    }
}
