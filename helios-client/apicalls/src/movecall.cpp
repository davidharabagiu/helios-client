#include <QDebug>

#include "movecall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string MoveCall::s_kUrl                    = "move";
const std::string MoveCall::s_kTokenParam             = "token";
const std::string MoveCall::s_kPathParam              = "src";
const std::string MoveCall::s_kDestinationParam       = "dst";
const std::string MoveCall::s_kErrorInvalidSourcePath = "Invalid source path";
const std::string MoveCall::s_kErrorDestinationExists = "Destination already exists";

MoveCall::MoveCall(const std::string& authToken, const std::string& path, const std::string& destination,
                   const ApiCallbacks::MoveCallback& callback)
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
    m_request->setParameter(s_kDestinationParam, destination);
}

std::unique_ptr<UrlEncodedRequest> MoveCall::request()
{
    return std::move(m_request);
}

void MoveCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(*this);
}

void MoveCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
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
        if (replyStr == s_kErrorInvalidSourcePath)
        {
            m_callback(ApiCallStatus::INVALID_PATH);
        }
        else if (replyStr == s_kErrorDestinationExists)
        {
            m_callback(ApiCallStatus::FILE_ALREADY_EXISTS);
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR);
}
