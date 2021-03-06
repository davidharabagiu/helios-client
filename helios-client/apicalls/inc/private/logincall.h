#ifndef LOGINCALL_H
#define LOGINCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class LoginCall
 * @brief Manages a user authentication call
 */
class LoginCall : public ApiCall
{
public:
    LoginCall(const std::string& username, const std::string& password, const ApiCallbacks::LoginCallback& callback);

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
    ApiCallbacks::LoginCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Username parameter name
     */
    static const std::string s_kUsernameParam;

    /**
     * @brief Password parameter name
     */
    static const std::string s_kPasswordParam;

    /**
     * @brief Invalid username error message
     */
    static const std::string s_kErrorInvalidUsername;

    /**
     * @brief Invalid password error message
     */
    static const std::string s_kErrorInvalidPassword;
};

#endif  // LOGINCALL_H
