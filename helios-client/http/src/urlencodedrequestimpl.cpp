#include "urlencodedrequestimpl.h"

UrlEncodedRequestImpl::UrlEncodedRequestImpl(const std::string& url)
    : m_url(url)
{
}

void UrlEncodedRequestImpl::setUrl(const std::string& url)
{
    m_url = url;
}

void UrlEncodedRequestImpl::setHeaderValue(const std::string& name, const std::string& value)
{
    m_header[name] = value;
}

void UrlEncodedRequestImpl::setParameter(const std::string& name, const std::string& value)
{
    m_urlQuery.addQueryItem(QString(name.c_str()), QString(value.c_str()));
}

std::string UrlEncodedRequestImpl::url() const
{
    return m_url;
}

std::map<std::string, std::string> UrlEncodedRequestImpl::header() const
{
    return m_header;
}

const QUrlQuery& UrlEncodedRequestImpl::urlQuery() const
{
    return m_urlQuery;
}
