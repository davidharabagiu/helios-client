#include <QDebug>

#include "registerusercall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string RegisterUserCall::s_kUrl                       = "register";
const std::string RegisterUserCall::s_kUsernameParam             = "username";
const std::string RegisterUserCall::s_kPasswordParam             = "password";
const std::string RegisterUserCall::s_kErrorUsernameAlreadyTaken = "Username already taken";
const std::string RegisterUserCall::s_kErrorInvalidUsername      = "Invalid username";
const std::string RegisterUserCall::s_kErrorInvalidPassword      = "Invalid password";

RegisterUserCall::RegisterUserCall(const std::string& username, const std::string& password,
                                   const ApiCallbacks::RegisterUserCallback& callback)
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

std::shared_ptr<UrlEncodedRequest> RegisterUserCall::request()
{
    return m_request;
}

void RegisterUserCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void RegisterUserCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS);
        return;
    }
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorUsernameAlreadyTaken)
        {
            m_callback(ApiCallStatus::USERNAME_ALREADY_EXISTS);
            return;
        }
        else if (replyStr == s_kErrorInvalidUsername)
        {
            m_callback(ApiCallStatus::INVALID_USERNAME);
            return;
        }
        else if (replyStr == s_kErrorInvalidPassword)
        {
            m_callback(ApiCallStatus::INVALID_PASSWORD);
            return;
        }
    }

    qCritical() << "Unhandled HTTP reply with status" << static_cast<int>(status) << "and content" << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR);
}
