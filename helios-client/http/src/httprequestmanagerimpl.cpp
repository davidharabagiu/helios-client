#include "httprequestmanagerimpl.h"
#include "qhttprequestmanagerimpl.h"

HttpRequestManagerImpl::HttpRequestManagerImpl()
    : m_privateImpl(new QHttpRequestManagerImpl())
{
}

void HttpRequestManagerImpl::post(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback)
{
    m_privateImpl->post(request, callback);
}

void HttpRequestManagerImpl::post(std::shared_ptr<FormDataRequest> request, const HttpReplyCallback& callback)
{
    m_privateImpl->post(request, callback);
}

void HttpRequestManagerImpl::get(std::shared_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback)
{
    m_privateImpl->get(request, callback);
}

void HttpRequestManagerImpl::setIgnoreSslErrors(bool value)
{
    m_privateImpl->setIgnoreSslErrors(value);
}
