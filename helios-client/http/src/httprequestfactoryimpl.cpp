#include "httprequestfactoryimpl.h"
#include "urlencodedrequestimpl.h"
#include "formdatarequestimpl.h"

std::unique_ptr<UrlEncodedRequest> HttpRequestFactoryImpl::createUrlEncodedRequest(const std::string& url) const
{
    return std::make_unique<UrlEncodedRequestImpl>(url);
}

std::unique_ptr<FormDataRequest> HttpRequestFactoryImpl::createFormDataRequest(const std::string& url) const
{
    return std::make_unique<FormDataRequestImpl>(url);
}
