#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "formdatarequestimpl.h"

class FormDataRequestTests : public testing::Test
{
protected:
    void SetUp() override
    {
        m_request = std::make_shared<FormDataRequestImpl>(m_kUrl);
    }

    std::shared_ptr<FormDataRequestImpl> m_request;
    const std::string                    m_kUrl  = "http://www.test1.com/";
    const std::string                    m_kUrl2 = "http://www.test2.net/";
};

TEST_F(FormDataRequestTests, ChangeUrl)
{
    EXPECT_EQ(m_request->url(), m_kUrl);
    m_request->setUrl(m_kUrl2);
    EXPECT_EQ(m_request->url(), m_kUrl2);
}

TEST_F(FormDataRequestTests, AddHeaderData)
{
    EXPECT_EQ(m_request->header().size(), 0);
    m_request->setHeaderValue("key1", "value1");
    m_request->setHeaderValue("key2", "value2");
    auto header = m_request->header();
    EXPECT_EQ(header.size(), 2);
    EXPECT_EQ(header["key1"], "value1");
    EXPECT_EQ(header["key2"], "value2");
}

TEST_F(FormDataRequestTests, MultiPartValid)
{
    EXPECT_NE(m_request->multiPart(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*m_request->multiPart());
    m_request->setPart("key", FormDataRequest::HttpPartType::TEXT, "value");
    EXPECT_NE(m_request->multiPart(), nullptr);
    EXPECT_NO_FATAL_FAILURE(*m_request->multiPart());
}
