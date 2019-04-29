#include <QNetworkReply>

#include "httpreplylistener.h"

HttpReplyListener::HttpReplyListener(int id, QNetworkReply* reply, const std::function<void(int)>& callback)
    : m_id(id)
    , m_reply(reply)
    , m_callback(callback)
{
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyReceived()));
}

HttpReplyListener::~HttpReplyListener()
{
    m_reply->deleteLater();
}

QNetworkReply* HttpReplyListener::reply() const
{
    return m_reply;
}

void HttpReplyListener::onReplyReceived() const
{
    m_callback(m_id);
}
