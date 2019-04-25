#include <QDebug>

#include "registerusercall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"

const std::string RegisterUserCall::s_kUrl                       = "http://127.0.0.1:8000/register";
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

    m_request = requestFactory->createUrlEncodedRequest(s_kUrl);
    m_request->setParameter(s_kUsernameParam, username);
    m_request->setParameter(s_kPasswordParam, password);
}

std::unique_ptr<UrlEncodedRequest> RegisterUserCall::request()
{
    return std::move(m_request);
}

void RegisterUserCall::send(const ApiCallVisitor& visitor)
{
    visitor.visit(*this);
}

void RegisterUserCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS);
    }
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorUsernameAlreadyTaken)
        {
            m_callback(ApiCallStatus::USERNAME_ALREADY_EXISTS);
        }
        else if (replyStr == s_kErrorInvalidUsername)
        {
            m_callback(ApiCallStatus::INVALID_USERNAME);
        }
        else if (replyStr == s_kErrorInvalidPassword)
        {
            m_callback(ApiCallStatus::INVALID_PASSWORD);
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR);
}
