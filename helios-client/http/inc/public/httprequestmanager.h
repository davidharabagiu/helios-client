#ifndef HTTPREQUESTMANAGER_H
#define HTTPREQUESTMANAGER_H

#include <functional>
#include <vector>
#include <cstdint>
#include <memory>

#include "httpstatus.h"

class UrlEncodedRequest;
class FormDataRequest;

/**
 * @brief Http reply callback containing the reply data
 * @param status - Reply status
 * @param content - Reply content
 */
using HttpReplyCallback = std::function<void(HttpStatus status, const std::vector<uint8_t>& reply)>;

/**
 * @class HttpRequestManager
 * @brief Provides a way to send http requests to a server and get back replies
 */
class HttpRequestManager
{
public:
    /**
     * @brief Destructor
     */
    virtual ~HttpRequestManager() = default;

    /**
     * @brief Send an http post request with url encoded parameters
     * @param request - Request
     * @param callback - Reply callback
     */
    virtual void post(std::unique_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback) = 0;

    /**
     * @brief Send an http post request with multipart form data parameters
     * @param request - Request
     * @param callback - Reply callback
     */
    virtual void post(std::unique_ptr<FormDataRequest> request, const HttpReplyCallback& callback) = 0;

    /**
     * @brief Send an http get request with url encoded parameters
     * @param request - Request
     * @param callback - Reply callback
     */
    virtual void get(std::unique_ptr<UrlEncodedRequest> request, const HttpReplyCallback& callback) = 0;
};

#endif  // HTTPREQUESTMANAGER_H
