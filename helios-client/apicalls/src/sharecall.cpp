#include <QDebug>

#include "sharecall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string ShareCall::s_kUrl              = "share";
const std::string ShareCall::s_kTokenParam       = "token";
const std::string ShareCall::s_kUsernameParam    = "to";
const std::string ShareCall::s_kPathParam        = "path";
const std::string ShareCall::s_kErrorInvalidUser = "Invalid user";
const std::string ShareCall::s_kErrorInvalidPath = "Invalid path";

ShareCall::ShareCall(const std::string& authToken, const std::string& username, const std::string& path,
                     const ApiCallbacks::ShareCallback& callback)
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
    m_request->setParameter(s_kPathParam, path);
}

std::shared_ptr<UrlEncodedRequest> ShareCall::request()
{
    return m_request;
}

void ShareCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void ShareCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
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
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorInvalidUser)
    {
        m_callback(ApiCallStatus::INVALID_USERNAME);
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
