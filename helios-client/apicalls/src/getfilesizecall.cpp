#include <QDebug>
#include <exception>

#include "getfilesizecall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string GetFileSizeCall::s_kUrl              = "size";
const std::string GetFileSizeCall::s_kTokenParam       = "token";
const std::string GetFileSizeCall::s_kPathParam        = "path";
const std::string GetFileSizeCall::s_kErrorInvalidPath = "Invalid path";

GetFileSizeCall::GetFileSizeCall(const std::string& authToken, const std::string& path,
                                 const ApiCallbacks::GetFileSizeCallback& callback)
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

std::unique_ptr<UrlEncodedRequest> GetFileSizeCall::request()
{
    return std::move(m_request);
}

void GetFileSizeCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(*this);
}

void GetFileSizeCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        try
        {
            m_callback(ApiCallStatus::SUCCESS, std::stoull(replyStr));
        }
        catch (const std::exception& exception)
        {
            qCritical() << "Exception while parsing reply " << replyStr.c_str() << ": " << exception.what();
            m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, 0);
        }
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, 0);
    }
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorInvalidPath)
        {
            m_callback(ApiCallStatus::INVALID_PATH, 0);
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, 0);
}
