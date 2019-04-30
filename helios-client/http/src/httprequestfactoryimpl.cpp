#include "httprequestfactoryimpl.h"
#include "urlencodedrequestimpl.h"
#include "formdatarequestimpl.h"

std::shared_ptr<UrlEncodedRequest> HttpRequestFactoryImpl::createUrlEncodedRequest(const std::string& url) const
{
    return std::make_shared<UrlEncodedRequestImpl>(url);
}

std::shared_ptr<FormDataRequest> HttpRequestFactoryImpl::createFormDataRequest(const std::string& url) const
{
    return std::make_shared<FormDataRequestImpl>(url);
}
