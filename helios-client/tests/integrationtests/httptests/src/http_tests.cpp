#include <gtest/gtest.h>
#include <QJsonDocument>
#include <QDebug>
#include <cstdlib>
#include <chrono>

#include "testapplication.h"
#include "httprequestmanager.h"
#include "httprequestfactory.h"
#include "formdatarequest.h"
#include "urlencodedrequest.h"
#include "single.h"
#include "dependencyinjector.h"
#include "typeconversions.h"

class HttpTests : public testing::Test
{
protected:
    void SetUp() override
    {
        m_requestFactory = Single<DependencyInjector>::instance().getInstance<HttpRequestFactory>();
        m_requestManager = Single<DependencyInjector>::instance().getInstance<HttpRequestManager>();
    }

    std::shared_ptr<HttpRequestFactory> m_requestFactory;
    std::shared_ptr<HttpRequestManager> m_requestManager;
    const std::string                   m_kUrl            = "https://httpbin.org/get";
    const std::string                   m_kUrl2           = "http://www.blahblah.blah/";
    const std::string                   m_kUrl3           = "https://duckduckgo.com/";
    const std::string                   m_kUrl4           = "https://httpbin.org/post";
    const int64_t                       m_kRequestTimeout = 2000;
};

TEST_F(HttpTests, HttpGet)
{
    auto request = m_requestFactory->createUrlEncodedRequest(m_kUrl);

    EXPECT_NE(request.get(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*request);

    request->setHeaderValue("header1", "blah");
    request->setHeaderValue("header2", "blah blah");
    request->setParameter("param1", "value1");
    request->setParameter("param2", "value2");

    bool ready = false;

    m_requestManager->get(request, [&ready](HttpStatus status, const std::vector<uint8_t>& reply, bool success) {
        EXPECT_TRUE(success);
        EXPECT_EQ(status, HttpStatus::OK);

        QJsonParseError jsonError;
        auto            json = QJsonDocument::fromJson(
            QByteArray(reinterpret_cast<const char*>(reply.data()), safe_integral_cast<int>(reply.size())), &jsonError);
        EXPECT_EQ(jsonError.error, QJsonParseError::NoError);

        EXPECT_EQ(json["url"].toString(), "https://httpbin.org/get?param1=value1&param2=value2");
        EXPECT_EQ(json["headers"]["Header1"].toString(), "blah");
        EXPECT_EQ(json["headers"]["Header2"].toString(), "blah blah");
        EXPECT_EQ(json["args"]["param1"].toString(), "value1");
        EXPECT_EQ(json["args"]["param2"].toString(), "value2");

        ready = true;
    });

    TestApplication::processEventsUntilPredicate([&ready] { return ready; }, m_kRequestTimeout, false);
}

TEST_F(HttpTests, WrongUrlRequest)
{
    auto request = m_requestFactory->createUrlEncodedRequest(m_kUrl2);

    EXPECT_NE(request.get(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*request);

    bool ready = false;

    m_requestManager->get(request,
                          [&ready](HttpStatus /*status*/, const std::vector<uint8_t>& /*reply*/, bool success) {
                              EXPECT_FALSE(success);
                              ready = true;
                          });

    TestApplication::processEventsUntilPredicate([&ready] { return ready; }, m_kRequestTimeout, false);
}

TEST_F(HttpTests, MultipleRequestsInParallel)
{
    auto request1 = m_requestFactory->createUrlEncodedRequest(m_kUrl);
    auto request2 = m_requestFactory->createUrlEncodedRequest(m_kUrl2);
    auto request3 = m_requestFactory->createUrlEncodedRequest(m_kUrl3);

    EXPECT_NE(request1.get(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*request1);
    EXPECT_NE(request2.get(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*request2);
    EXPECT_NE(request3.get(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*request3);

    bool ready1 = false;
    bool ready2 = false;
    bool ready3 = false;

    m_requestManager->get(request1, [&ready1](HttpStatus status, const std::vector<uint8_t>& /*reply*/, bool success) {
        EXPECT_TRUE(success);
        EXPECT_EQ(status, HttpStatus::OK);
        qDebug() << "Reply 1 received";
        ready1 = true;
    });

    m_requestManager->get(request2,
                          [&ready2](HttpStatus /*status*/, const std::vector<uint8_t>& /*reply*/, bool success) {
                              EXPECT_FALSE(success);
                              qDebug() << "Reply 2 received";
                              ready2 = true;
                          });

    m_requestManager->get(request3, [&ready3](HttpStatus status, const std::vector<uint8_t>& /*reply*/, bool success) {
        EXPECT_TRUE(success);
        EXPECT_EQ(status, HttpStatus::OK);
        qDebug() << "Reply 3 received";
        ready3 = true;
    });

    TestApplication::processEventsUntilPredicate([&ready1, &ready2, &ready3] { return ready1 && ready2 && ready3; },
                                                 m_kRequestTimeout, false);
}

TEST_F(HttpTests, HttpPostUrlEncoded)
{
    auto request = m_requestFactory->createUrlEncodedRequest(m_kUrl4);

    EXPECT_NE(request.get(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*request);

    request->setHeaderValue("header1", "blah");
    request->setHeaderValue("header2", "blah blah");
    request->setParameter("param1", "value1");
    request->setParameter("param2", "value2");

    bool ready = false;

    m_requestManager->post(request, [&ready, this](HttpStatus status, const std::vector<uint8_t>& reply, bool success) {
        EXPECT_TRUE(success);
        EXPECT_EQ(status, HttpStatus::OK);

        QJsonParseError jsonError;
        auto            json = QJsonDocument::fromJson(
            QByteArray(reinterpret_cast<const char*>(reply.data()), safe_integral_cast<int>(reply.size())), &jsonError);
        EXPECT_EQ(jsonError.error, QJsonParseError::NoError);

        EXPECT_EQ(json["url"].toString().toStdString(), m_kUrl4);
        EXPECT_EQ(json["headers"]["Header1"].toString(), "blah");
        EXPECT_EQ(json["headers"]["Header2"].toString(), "blah blah");
        EXPECT_EQ(json["form"]["param1"].toString(), "value1");
        EXPECT_EQ(json["form"]["param2"].toString(), "value2");

        ready = true;
    });

    TestApplication::processEventsUntilPredicate([&ready] { return ready; }, m_kRequestTimeout, false);
}

TEST_F(HttpTests, HttpPostFormData)
{
    auto request = m_requestFactory->createFormDataRequest(m_kUrl4);

    EXPECT_NE(request.get(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*request);

    request->setHeaderValue("header1", "blah");
    request->setHeaderValue("header2", "blah blah");
    request->setPart("param1", FormDataRequest::HttpPartType::TEXT, "value1");
    request->setPart("param2", FormDataRequest::HttpPartType::FILE, "\x41\x42\xF4\xFF");

    bool ready = false;

    m_requestManager->post(request, [&ready, this](HttpStatus status, const std::vector<uint8_t>& reply, bool success) {
        EXPECT_TRUE(success);
        EXPECT_EQ(status, HttpStatus::OK);

        QJsonParseError jsonError;
        auto            json = QJsonDocument::fromJson(
            QByteArray(reinterpret_cast<const char*>(reply.data()), safe_integral_cast<int>(reply.size())), &jsonError);
        EXPECT_EQ(jsonError.error, QJsonParseError::NoError);

        EXPECT_EQ(json["url"].toString().toStdString(), m_kUrl4);
        EXPECT_EQ(json["headers"]["Header1"].toString(), "blah");
        EXPECT_EQ(json["headers"]["Header2"].toString(), "blah blah");
        EXPECT_EQ(json["form"]["param1"].toString(), "value1");
        EXPECT_EQ(json["form"]["param2"].toString(), "\x41\x42\xF4\xFF");

        ready = true;
    });

    TestApplication::processEventsUntilPredicate([&ready] { return ready; }, m_kRequestTimeout, false);
}
