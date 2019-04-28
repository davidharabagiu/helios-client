#include <QDebug>

#include "begindownloadcall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string BeginDownloadCall::s_kUrl              = "begindownload";
const std::string BeginDownloadCall::s_kTokenParam       = "token";
const std::string BeginDownloadCall::s_kPathParam        = "path";
const std::string BeginDownloadCall::s_kErrorInvalidPath = "Invalid path";

BeginDownloadCall::BeginDownloadCall(const std::string& authToken, const std::string& path,
                                     const ApiCallbacks::BeginUploadCallback& callback)
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
    m_request->setParameter(s_kPathParam, path);
}

std::unique_ptr<UrlEncodedRequest> BeginDownloadCall::request()
{
    return std::move(m_request);
}

void BeginDownloadCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(*this);
}

void BeginDownloadCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS, replyStr);
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, std::string());
    }
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorInvalidPath)
        {
            m_callback(ApiCallStatus::INVALID_PATH, std::string());
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, std::string());
}
