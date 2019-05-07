#include <QDebug>

#include "endtransfercall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string EndTransferCall::s_kUrl                    = "endtransfer";
const std::string EndTransferCall::s_kTokenParam             = "token";
const std::string EndTransferCall::s_kTranferIdParam         = "transferId";
const std::string EndTransferCall::s_kErrorInvalidTransferId = "Invalid transfer id";

EndTransferCall::EndTransferCall(const std::string& authToken, const std::string& transferId,
                                 const ApiCallbacks::EndTransferCallback& callback)
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
}

std::shared_ptr<UrlEncodedRequest> EndTransferCall::request()
{
    return m_request;
}

void EndTransferCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void EndTransferCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
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
        if (replyStr == s_kErrorInvalidTransferId)
        {
            m_callback(ApiCallStatus::INVALID_TRANSFER_ID);
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR);
}
