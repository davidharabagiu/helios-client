#include <QDebug>

#include "setuserkeycall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string SetUserKeyCall::s_kUrl        = "setuserkey";
const std::string SetUserKeyCall::s_kTokenParam = "token";
const std::string SetUserKeyCall::s_kKeyParam   = "key";

SetUserKeyCall::SetUserKeyCall(const std::string& authToken, const std::string& key,
                               const ApiCallbacks::SetUserKeyCallback& callback)
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

    m_request = requestFactory->createFormDataRequest(config->get(ConfigKeys::kServerUrlKey).toString().toStdString() +
                                                      "/" + s_kUrl);
    m_request->setHeaderValue(s_kTokenParam, authToken);
    m_request->setPart(s_kKeyParam, FormDataRequest::HttpPartType::FILE, key);
}

std::shared_ptr<FormDataRequest> SetUserKeyCall::request()
{
    return m_request;
}

void SetUserKeyCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void SetUserKeyCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS);
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED);
    }
    else
    {
        std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());
        qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                    << replyStr.c_str();
        m_callback(ApiCallStatus::UNKNOWN_ERROR);
    }
}
