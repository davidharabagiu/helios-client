#include <QNetworkAccessManager>
#include <QDebug>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QUrlQuery>
#include <cassert>

#include "httprequestmanagerimpl.h"
#include "httpreplylistener.h"
#include "urlencodedrequest.h"
#include "urlencodedrequestprivate.h"
#include "formdatarequest.h"
#include "formdatarequestprivate.h"
#include "typeconversions.h"

const std::string HttpRequestManagerImpl::s_kUrlSeparator = "/";

HttpRequestManagerImpl::HttpRequestManagerImpl()
    : m_networkAccessManager(new QNetworkAccessManager())
    , m_lastAssignedRequestId(-1)
{
}

void HttpRequestManagerImpl::post(std::unique_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback)
{
    auto _request = dynamic_unique_cast<UrlEncodedRequestPrivate>(std::move(request));
    assert(_request);

    QNetworkRequest networkRequest(QUrl(QString(_request->url().c_str())));

    collectHeaderValues(_request->header(), networkRequest);

    const auto& urlQuery = _request->urlQuery().query();

    std::shared_ptr<QNetworkReply> reply(m_networkAccessManager->post(networkRequest, urlQuery.toUtf8()));
    addPendingReply(reply, callback);
}

void HttpRequestManagerImpl::post(std::unique_ptr<FormDataRequest> request, const HttpReplyCallback& callback)
{
    auto _request = dynamic_unique_cast<FormDataRequestPrivate>(std::move(request));
    assert(_request);

    QNetworkRequest networkRequest(QUrl(QString(_request->url().c_str())));

    collectHeaderValues(_request->header(), networkRequest);

    std::shared_ptr<QNetworkReply> reply(m_networkAccessManager->post(networkRequest, _request->multiPart().get()));
    addPendingReply(reply, callback);
}

void HttpRequestManagerImpl::get(std::unique_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback)
{
    auto _request = dynamic_unique_cast<UrlEncodedRequestPrivate>(std::move(request));
    assert(_request);

    auto            url = _request->url() + s_kUrlSeparator + _request->urlQuery().query().toStdString();
    QNetworkRequest networkRequest(QUrl(QString(url.c_str())));

    collectHeaderValues(_request->header(), networkRequest);

    std::shared_ptr<QNetworkReply> reply(m_networkAccessManager->get(networkRequest));
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

    const auto&       reply    = std::get<0>(it->second)->reply();
    HttpReplyCallback callback = std::get<1>(it->second);

    const auto& replyString = reply->readAll().toStdString();

    callback(static_cast<HttpStatus>(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()),
             std::vector<uint8_t>(replyString.cbegin(), replyString.cend()));
    m_pendingReplies.erase(it);
}

void HttpRequestManagerImpl::addPendingReply(const std::shared_ptr<QNetworkReply>& reply,
                                             const HttpReplyCallback&              callback)
{
    auto listener =
        std::make_shared<HttpReplyListener>(++m_lastAssignedRequestId, reply, [this](int id) { repliedReceived(id); });
    m_pendingReplies.emplace(m_lastAssignedRequestId, std::make_tuple(listener, callback));
}

void HttpRequestManagerImpl::collectHeaderValues(const std::map<std::string, std::string>& values,
                                                 QNetworkRequest&                          result)
{
    for (const auto& param : values)
    {
        result.setRawHeader(QByteArray(param.first.data(), safe_integral_cast<int>(param.first.size())),
                            QByteArray(param.second.data(), safe_integral_cast<int>(param.second.size())));
    }
}
