#include <QDebug>
#include <QByteArray>

#include "sharekeycall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"
#include "typeconversions.h"

const std::string ShareKeyCall::s_kUrl              = "sharekey";
const std::string ShareKeyCall::s_kTokenParam       = "token";
const std::string ShareKeyCall::s_kUsernameParam    = "to";
const std::string ShareKeyCall::s_kKeyNameParam     = "name";
const std::string ShareKeyCall::s_kKeyLengthParam   = "length";
const std::string ShareKeyCall::s_kKeyContentParam  = "data";
const std::string ShareKeyCall::s_kErrorInvalidUser = "Invalid user";

ShareKeyCall::ShareKeyCall(const std::string& authToken, const std::string& username, const std::string& keyName,
                           uint16_t keyLength, const std::vector<uint8_t>& keyContent,
                           const ApiCallbacks::ShareKeyCallback& callback)
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
    m_request->setPart(s_kUsernameParam, FormDataRequest::HttpPartType::TEXT, username);
    m_request->setPart(s_kKeyNameParam, FormDataRequest::HttpPartType::TEXT, keyName);
    m_request->setPart(s_kKeyLengthParam, FormDataRequest::HttpPartType::TEXT, std::to_string(keyLength));
    m_request->setPart(s_kKeyContentParam, FormDataRequest::HttpPartType::FILE,
                       QByteArray::fromRawData(reinterpret_cast<const char*>(keyContent.data()),
                                               safe_integral_cast<int>(keyContent.size()))
                           .toBase64()
                           .toStdString());
}

std::shared_ptr<FormDataRequest> ShareKeyCall::request()
{
    return m_request;
}

void ShareKeyCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void ShareKeyCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
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
    else if (status == HttpStatus::BAD_REQUEST && replyStr == s_kErrorInvalidUser)
    {
        m_callback(ApiCallStatus::INVALID_USERNAME);
    }
    else
    {
        qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                    << replyStr.c_str();
        m_callback(ApiCallStatus::UNKNOWN_ERROR);
    }
}
