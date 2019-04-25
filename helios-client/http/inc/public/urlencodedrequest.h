#ifndef URLENCODEDREQUEST_H
#define URLENCODEDREQUEST_H

#include <string>

/**
 * @class UrlEncodedRequest
 * @brief Interface of an http request with url encoded parameters
 */
class UrlEncodedRequest
{
public:
    /**
     * @brief Destructor
     */
    virtual ~UrlEncodedRequest() = default;

    /**
     * @brief Setter for url
     * @param url - New url value
     */
    virtual void setUrl(const std::string& url) = 0;

    /**
     * @brief Set a value from the header. If this header already exists, its value will be replaced.
     * @param name - header field name
     * @param value - header field value
     */
    virtual void setHeaderValue(const std::string& name, const std::string& value) = 0;

    /**
     * @brief Set a parameter value. If this parameter already exists, its value will be replaced.
     * @param name - parameter name
     * @param value - parameter value
     */
    virtual void setParameter(const std::string& name, const std::string& value) = 0;
};

#endif  // URLENCODEDREQUEST_H
