#include <gtest/gtest.h>
#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>

#include "httpreplylistener.h"
#include "testapplication.h"

class HttpReplyListenerTests : public testing::Test
{
protected:
    void SetUp() override
    {
        m_networkAccessManager = std::make_unique<QNetworkAccessManager>();
        m_request              = std::make_shared<QNetworkRequest>(m_kUrl);
    }

    std::unique_ptr<QNetworkAccessManager> m_networkAccessManager;
    std::shared_ptr<QNetworkRequest>       m_request;
    const QUrl                             m_kUrl            = QString("https://httpbin.org/get?key=value");
    const int64_t                          m_kRequestTimeout = 2000;
};

TEST_F(HttpReplyListenerTests, CallbackCalledOnHttpReplyReceived)
{
    auto reply = m_networkAccessManager->get(*m_request);

    bool replyReceived = false;

    HttpReplyListener listener(10, reply, [&replyReceived](int id) {
        EXPECT_EQ(id, 10);
        replyReceived = true;
    });

    TestApplication::processEventsUntilPredicate([&replyReceived] { return replyReceived; }, m_kRequestTimeout, false);

    EXPECT_TRUE(reply->isFinished());
    EXPECT_EQ(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute), 200);

    auto json = QJsonDocument::fromJson(reply->readAll());
    EXPECT_EQ(json["args"]["key"].toString(), QString("value"));
    EXPECT_EQ(json["url"].toString(), m_kUrl.toString());
}
