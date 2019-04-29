#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "urlencodedrequestimpl.h"

class UrlEncodedRequestTests : public testing::Test
{
protected:
    void SetUp() override
    {
        m_request = std::make_shared<UrlEncodedRequestImpl>(m_kUrl);
    }

    std::shared_ptr<UrlEncodedRequestImpl> m_request;
    const std::string                      m_kUrl  = "http://www.test1.com/";
    const std::string                      m_kUrl2 = "http://www.test2.net/";
};

TEST_F(UrlEncodedRequestTests, ChangeUrl)
{
    EXPECT_EQ(m_request->url(), m_kUrl);
    m_request->setUrl(m_kUrl2);
    EXPECT_EQ(m_request->url(), m_kUrl2);
}

TEST_F(UrlEncodedRequestTests, AddHeaderData)
{
    EXPECT_EQ(m_request->header().size(), 0);
    m_request->setHeaderValue("key1", "value1");
    m_request->setHeaderValue("key2", "value2");
    auto header = m_request->header();
    EXPECT_EQ(header.size(), 2);
    EXPECT_EQ(header["key1"], "value1");
    EXPECT_EQ(header["key2"], "value2");
}

TEST_F(UrlEncodedRequestTests, AddParameters)
{
    EXPECT_TRUE(m_request->urlQuery().isEmpty());
    m_request->setParameter("key1", "value1");
    m_request->setParameter("key2", "value2");
    EXPECT_EQ(m_request->urlQuery().query(), "key1=value1&key2=value2");
}
