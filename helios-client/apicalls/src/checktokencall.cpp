#include <QDebug>

#include "checktokencall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string CheckTokenCall::s_kUrl           = "checktoken";
const std::string CheckTokenCall::s_kUsernameParam = "username";
const std::string CheckTokenCall::s_kTokenParam    = "token";

CheckTokenCall::CheckTokenCall(const std::string& username, const std::string& token,
                               const ApiCallbacks::CheckTokenCallback& callback)
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
    m_request->setParameter(s_kUsernameParam, username);
}

std::shared_ptr<UrlEncodedRequest> CheckTokenCall::request()
{
    return m_request;
}

void CheckTokenCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void CheckTokenCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS);
        return;
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED);
        return;
    }

    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());
    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR);
}
