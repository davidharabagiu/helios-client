#include <QNetworkReply>

#include "httpreplylistener.h"

HttpReplyListener::HttpReplyListener(int id, const std::shared_ptr<QNetworkReply>& reply,
                                     std::function<void(int)> callback)
    : m_id(id)
    , m_reply(reply)
    , m_callback(callback)
{
    connect(reply.get(), SIGNAL(finished()), this, SLOT(onReplyReceived()));
}

std::shared_ptr<QNetworkReply> HttpReplyListener::reply() const
{
    return m_reply;
}

void HttpReplyListener::onReplyReceived() const
{
    m_callback(m_id);
}
