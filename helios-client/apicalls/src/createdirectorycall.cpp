#include <QDebug>

#include "createdirectorycall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string CreateDirectoryCall::s_kUrl                  = "mkdir";
const std::string CreateDirectoryCall::s_kTokenParam           = "token";
const std::string CreateDirectoryCall::s_kPathParam            = "path";
const std::string CreateDirectoryCall::s_kErrorDirectoryExists = "Directory already exists";
const std::string CreateDirectoryCall::s_kErrorInvalidPath     = "Invalid path";

CreateDirectoryCall::CreateDirectoryCall(const std::string& authToken, const std::string& path,
                                         const ApiCallbacks::CreateDirectoryCallback& callback)
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

std::unique_ptr<UrlEncodedRequest> CreateDirectoryCall::request()
{
    return std::move(m_request);
}

void CreateDirectoryCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(*this);
}

void CreateDirectoryCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
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
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorDirectoryExists)
        {
            m_callback(ApiCallStatus::FILE_ALREADY_EXISTS);
        }
        else if (replyStr == s_kErrorInvalidPath)
        {
            m_callback(ApiCallStatus::INVALID_PATH);
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR);
}
