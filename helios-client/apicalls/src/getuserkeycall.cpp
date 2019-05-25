#include <QDebug>

#include "getuserkeycall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string GetUserKeyCall::s_kUrl              = "getuserkey";
const std::string GetUserKeyCall::s_kTokenParam       = "token";
const std::string GetUserKeyCall::s_kUsernameParam    = "user";
const std::string GetUserKeyCall::s_kErrorInvalidUser = "Invalid user";
const std::string GetUserKeyCall::s_kErrorNoKey       = "User did not specify public key";

GetUserKeyCall::GetUserKeyCall(const std::string& authToken, const std::string& username,
                               const ApiCallbacks::GetUserKeyCallback& callback)
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
    m_request->setParameter(s_kUsernameParam, username);
}

std::shared_ptr<UrlEncodedRequest> GetUserKeyCall::request()
{
    return m_request;
}

void GetUserKeyCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void GetUserKeyCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS, replyStr);
        return;
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, "");
        return;
    }
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorInvalidUser)
        {
            m_callback(ApiCallStatus::INVALID_USERNAME, "");
            return;
        }
        if (replyStr == s_kErrorNoKey)
        {
            m_callback(ApiCallStatus::NO_KEY_SPECIFIED, "");
            return;
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, 0);
}
