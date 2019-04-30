#include <QDebug>

#include "logincall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string LoginCall::s_kUrl                  = "login";
const std::string LoginCall::s_kUsernameParam        = "username";
const std::string LoginCall::s_kPasswordParam        = "password";
const std::string LoginCall::s_kErrorInvalidUsername = "Invalid username";
const std::string LoginCall::s_kErrorInvalidPassword = "Invalid password";

LoginCall::LoginCall(const std::string& username, const std::string& password,
                     const ApiCallbacks::LoginCallback& callback)
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
    m_request->setParameter(s_kUsernameParam, username);
    m_request->setParameter(s_kPasswordParam, password);
}

std::shared_ptr<UrlEncodedRequest> LoginCall::request()
{
    return m_request;
}

void LoginCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(*this);
}

void LoginCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS, replyStr);
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        if (replyStr == s_kErrorInvalidUsername)
        {
            m_callback(ApiCallStatus::INVALID_USERNAME, std::string());
        }
        else if (replyStr == s_kErrorInvalidPassword)
        {
            m_callback(ApiCallStatus::INVALID_PASSWORD, std::string());
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, std::string());
}
