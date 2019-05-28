#ifndef SHARECALL_H
#define SHARECALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class ShareCall
 * @brief Manages a file share call
 */
class ShareCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param authToken - Authentication token
     * @param username - Name of the user which will receive the file
     * @param path - Path of the file to share
     * @param callback - ShareCallback
     */
    ShareCall(const std::string& authToken, const std::string& username, const std::string& path,
              const ApiCallbacks::ShareCallback& callback);

public:
    /**
     * @brief Returns the http request
     * @return std::shared_ptr<UrlEncodedRequest>
     */
    std::shared_ptr<UrlEncodedRequest> request();

public:  // from ApiCall
    void send(std::shared_ptr<ApiCallVisitor> visitor) override;
    void receive(HttpStatus status, const std::vector<uint8_t>& reply) override;

private:
    /**
     * @brief Http request
     */
    std::shared_ptr<UrlEncodedRequest> m_request;

    /**
     * @brief Callback
     */
    ApiCallbacks::ShareCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief Destination user parameter name
     */
    static const std::string s_kUsernameParam;

    /**
     * @brief File path parameter name
     */
    static const std::string s_kPathParam;

    /**
     * @brief Invalid user error message
     */
    static const std::string s_kErrorInvalidUser;

    /**
     * @brief Invalid path error message
     */
    static const std::string s_kErrorInvalidPath;
};

#endif  // SHARECALL_H
