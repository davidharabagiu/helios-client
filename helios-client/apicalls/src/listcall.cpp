#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDebug>
#include <tuple>
#include <optional>
#include <cstdint>

#include "listcall.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestfactory.h"
#include "apicallvisitor.h"
#include "httpstatus.h"
#include "config.h"
#include "configkeys.h"
#include "typeconversions.h"

const std::string ListCall::s_kUrl               = "list";
const std::string ListCall::s_kTokenParam        = "token";
const std::string ListCall::s_kPathParam         = "path";
const QString     ListCall::s_kFileNameJsonField = "name";
const QString     ListCall::s_kIsDirJsonField    = "isDir";
const QString     ListCall::s_kSizeJsonField     = "size";
const std::string ListCall::s_kErrorInvalidPath  = "Invalid path";

ListCall::ListCall(const std::string& authToken, const std::string& path, const ApiCallbacks::ListCallback& callback)
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

std::shared_ptr<UrlEncodedRequest> ListCall::request()
{
    return m_request;
}

void ListCall::send(std::shared_ptr<ApiCallVisitor> visitor)
{
    visitor->visit(this);
}

void ListCall::receive(HttpStatus status, const std::vector<uint8_t>& reply)
{
    std::string replyStr(reinterpret_cast<const char*>(reply.data()), reply.size());

    if (status == HttpStatus::OK)
    {
        QJsonParseError jsonError;
        auto            json = QJsonDocument::fromJson(
            QByteArray(reinterpret_cast<const char*>(reply.data()), safe_integral_cast<int>(reply.size())), &jsonError);
        if (jsonError.error != QJsonParseError::NoError)
        {
            qCritical() << "JSON parse error: " << jsonError.errorString();
            m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, {});
            return;
        }

        std::vector<std::tuple<std::string, bool, std::optional<uint64_t>>> files;

        auto jsonArray = json.array();
        for (const auto& jsonArrayElement : jsonArray)
        {
            if (!jsonArrayElement.isObject())
            {
                qCritical() << "Invalid json reply received: " << replyStr.c_str();
                m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, {});
            }
            auto jsonObject     = jsonArrayElement.toObject();
            auto jsonFileField  = jsonObject[s_kFileNameJsonField];
            auto jsonIsDirField = jsonObject[s_kIsDirJsonField];
            auto jsonSizeField  = jsonObject[s_kSizeJsonField];
            if (jsonFileField.type() != QJsonValue::String || jsonIsDirField.type() != QJsonValue::Bool ||
                (!jsonIsDirField.toBool() && jsonSizeField.type() != QJsonValue::Double))
            {
                qCritical() << "Invalid json reply received: " << replyStr.c_str();
                m_callback(ApiCallStatus::INVALID_REPLY_FORMAT, {});
            }
            files.emplace_back(jsonFileField.toString().toStdString(), jsonIsDirField.toBool(),
                               jsonIsDirField.toBool() ?
                                   std::nullopt :
                                   std::optional<uint64_t>(static_cast<uint64_t>(jsonSizeField.toDouble())));
        }

        m_callback(ApiCallStatus::SUCCESS, files);
        return;
    }
    else if (status == HttpStatus::UNAUTHORIZED)
    {
        m_callback(ApiCallStatus::UNAUTHORIZED, {});
        return;
    }
    else if (status == HttpStatus::BAD_REQUEST)
    {
        if (replyStr == s_kErrorInvalidPath)
        {
            m_callback(ApiCallStatus::INVALID_PATH, {});
            return;
        }
    }

    qCritical() << "Unhandled HTTP reply with status " << static_cast<int>(status) << " and content "
                << replyStr.c_str();
    m_callback(ApiCallStatus::UNKNOWN_ERROR, {});
}
