#include <QDebug>

#include "uploadcall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"

const std::string UploadCall::s_kUrl                    = "upload";
const std::string UploadCall::s_kTokenParam             = "token";
const std::string UploadCall::s_kTranferIdParam         = "transferId";
const std::string UploadCall::s_kOffsetParam            = "offset";
const std::string UploadCall::s_kFileContentParam       = "data";
const std::string UploadCall::s_kErrorInvalidTransferId = "Invalid transfer id";

UploadCall::UploadCall(const std::string& authToken, const std::string& transferId, uint64_t offset,
                       const std::vector<uint8_t>& content, const ApiCallbacks::UploadCallback& callback)
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
    m_request->setPart(s_kTranferIdParam, FormDataRequest::HttpPartType::TEXT, transferId);
    m_request->setPart(s_kOffsetParam, FormDataRequest::HttpPartType::TEXT, std::to_string(offset));
    m_request->setPart(s_kFileContentParam, FormDataRequest::HttpPartType::FILE, content);
}

std::unique_ptr<FormDataRequest> UploadCall::request()
{
    return std::move(m_request);
}

void UploadCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(*this);
}

void UploadCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
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
