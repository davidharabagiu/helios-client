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
    std::unique_ptr<UrlEncodedRequest> createUrlEncodedRequest(const std::string& url) const override;
    std::unique_ptr<FormDataRequest>   createFormDataRequest(const std::string& url) const override;
};

#endif  // HTTPREQUESTFACTORYIMPL_H
