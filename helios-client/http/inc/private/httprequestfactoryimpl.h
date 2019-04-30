#ifndef HTTPREQUESTFACTORYIMPL_H
#define HTTPREQUESTFACTORYIMPL_H

#include "httprequestfactory.h"

/**
 * @class HttpRequestFactoryImpl
 * @brief Private implementation of HttpRequestFactory
 */
class HttpRequestFactoryImpl : public HttpRequestFactory
{
public:
    HttpRequestFactoryImpl() = default;

public:  // from HttpRequestFactory
    std::shared_ptr<UrlEncodedRequest> createUrlEncodedRequest(const std::string& url) const override;
    std::shared_ptr<FormDataRequest>   createFormDataRequest(const std::string& url) const override;
};

#endif  // HTTPREQUESTFACTORYIMPL_H
