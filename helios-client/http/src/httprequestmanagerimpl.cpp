#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

#include "httprequestmanagerimpl.h"
#include "httpreplylistener.h"

HttpRequestManagerImpl::HttpRequestManagerImpl(const QString& baseUrl)
    : m_baseUrl(baseUrl)
    , m_networkAccessManager(new QNetworkAccessManager())
    , m_lastAssignedRequestId(-1)
{
}

void HttpRequestManagerImpl::post(const QString& url, const HttpParams& headerParams, const HttpParams& params,
                                  const HttpReplyCallback& callback)
{
    QNetworkRequest request(QUrl(m_baseUrl + "/" + url));
    for (const auto& param : headerParams)
    {
        request.setRawHeader(param.first.toUtf8(), param.second.toUtf8());
    }

    QUrlQuery query;
    for (const auto& param : params)
    {
        query.addQueryItem(param.first, param.second);
    }

    std::shared_ptr<QNetworkReply> reply(m_networkAccessManager->post(request, query.query().toUtf8()));
    addPendingReply(reply, callback);
}

void HttpRequestManagerImpl::postMultiPart(const QString& url, const HttpParams& headerParams, const HttpParts& parts,
                                           const HttpReplyCallback& callback)
{
    QNetworkRequest request(QUrl(m_baseUrl + "/" + url));
    for (const auto& param : headerParams)
    {
        request.setRawHeader(param.first.toUtf8(), param.second.toUtf8());
    }

    QHttpMultiPart multiPart;
    for (const auto& part : parts)
    {
        QHttpPart httpPart;
        httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"" + part.first + "\"");
        httpPart.setBody(part.second.second);
        switch (part.second.first)
        {
            case HttpPartType::TEXT:
            {
                httpPart.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
            }
            case HttpPartType::FILE:
            {
                httpPart.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
            }
            default:
            {
                qWarning() << "Unknown http part type: " << static_cast<int>(part.second.first);
                continue;
            }
        }
    }

    std::shared_ptr<QNetworkReply> reply(m_networkAccessManager->post(request, &multiPart));
    addPendingReply(reply, callback);
}

void HttpRequestManagerImpl::get(const QString& url, const HttpParams& headerParams, const HttpParams& params,
                                 const HttpReplyCallback& callback)
{
    QUrlQuery query;
    for (const auto& param : params)
    {
        query.addQueryItem(param.first, param.second);
    }

    QNetworkRequest request(QUrl(m_baseUrl + "/" + url + "/" + query.query()));
    for (const auto& param : headerParams)
    {
        request.setRawHeader(param.first.toUtf8(), param.second.toUtf8());
    }

    std::shared_ptr<QNetworkReply> reply(m_networkAccessManager->get(request));
    addPendingReply(reply, callback);
}

void HttpRequestManagerImpl::repliedReceived(int id)
{
    auto it = m_pendingReplies.find(id);
    if (it == m_pendingReplies.end())
    {
        qWarning() << "Received reply with unknown request id: " << id;
        return;
    }

    const auto&       reply    = it->second.first->reply();
    HttpReplyCallback callback = it->second.second;

    callback(static_cast<HttpStatus>(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()),
             reply->readAll());
    m_pendingReplies.erase(it);
}

void HttpRequestManagerImpl::addPendingReply(const std::shared_ptr<QNetworkReply>& reply,
                                             const HttpReplyCallback&              callback)
{
    auto listener =
        std::make_shared<HttpReplyListener>(++m_lastAssignedRequestId, reply, [this](int id) { repliedReceived(id); });
    m_pendingReplies.emplace(m_lastAssignedRequestId, std::make_pair(listener, callback));
}
