#include <QDebug>

#include "isdircall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string IsDirCall::s_kUrl        = "isdir";
const std::string IsDirCall::s_kTokenParam = "token";
const std::string IsDirCall::s_kPathParam  = "path";

IsDirCall::IsDirCall(const std::string& token, const std::string& path, const ApiCallbacks::IsDirCallback& callback)
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
    m_request->setHeaderValue(s_kTokenParam, token);
    m_request->setParameter(s_kPathParam, path);
}

std::shared_ptr<UrlEncodedRequest> IsDirCall::request()
{
    return m_request;
}

void IsDirCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void IsDirCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        if (replyStr == "true" || replyStr == "false")
        {
            m_callback(ApiCallStatus::SUCCESS, replyStr == "true");
        }
        else
        {
            m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, false);
        }
        return;
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, false);
        return;
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, false);
}
