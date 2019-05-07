#include <QDebug>

#include "downloadcall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string DownloadCall::s_kUrl                    = "download";
const std::string DownloadCall::s_kTokenParam             = "token";
const std::string DownloadCall::s_kTranferIdParam         = "transferId";
const std::string DownloadCall::s_kOffsetParam            = "offset";
const std::string DownloadCall::s_kChunkSizeParam         = "length";
const std::string DownloadCall::s_kErrorInvalidTransferId = "Invalid transfer id";

DownloadCall::DownloadCall(const std::string& authToken, const std::string& transferId, uint64_t offset,
                           const uint64_t& size, const ApiCallbacks::DownloadCallback& callback)
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
    m_request->setParameter(s_kTranferIdParam, transferId);
    m_request->setParameter(s_kOffsetParam, std::to_string(offset));
    m_request->setParameter(s_kChunkSizeParam, std::to_string(size));
}

std::shared_ptr<UrlEncodedRequest> DownloadCall::request()
{
    return m_request;
}

void DownloadCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void DownloadCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    if (status == HttpStatus::OK)
    {
        m_callback(ApiCallStatus::SUCCESS, reply);
    }

    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, {});
    }
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorInvalidTransferId)
        {
            m_callback(ApiCallStatus::INVALID_TRANSFER_ID, {});
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, {});
}
