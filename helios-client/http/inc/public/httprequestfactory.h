#ifndef HTTPREQUESTFACTORY_H
#define HTTPREQUESTFACTORY_H

#include <string>
#include <memory>

#include "urlencodedrequest.h"
#include "formdatarequest.h"

/**
 * @brief Abstract factory for http request objects
 */
class HttpRequestFactory
{
public:
    /**
     * Destructor
     */
    virtual ~HttpRequestFactory() = default;

    /**
     * @brief Create an http request with url encoded parameters
     * @param url - request url
     * @return std::shared_ptr<UrlEncodedRequest>
     */
    virtual std::shared_ptr<UrlEncodedRequest> createUrlEncodedRequest(const std::string& url) const = 0;

    /**
     * @brief Create an http request with form data parameters
     * @param url - request url
     * @return std::shared_ptr<FormDataRequest>
     */
    virtual std::shared_ptr<FormDataRequest> createFormDataRequest(const std::string& url) const = 0;
};

#endif  // HTTPREQUESTFACTORY_H
