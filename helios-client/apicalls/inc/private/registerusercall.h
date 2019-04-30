#ifndef REGISTERUSERCALL_H
#define REGISTERUSERCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class RegisterUserCall
 * @brief Manages a user registration call
 */
class RegisterUserCall : public ApiCall
{
public:
    RegisterUserCall(const std::string& username, const std::string& password,
                     const ApiCallbacks::RegisterUserCallback& callback);

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
    ApiCallbacks::RegisterUserCallback m_callback;

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
     * @brief Username already taken error message
     */
    static const std::string s_kErrorUsernameAlreadyTaken;

    /**
     * @brief Invalid username error message
     */
    static const std::string s_kErrorInvalidUsername;

    /**
     * @brief Invalid password error message
     */
    static const std::string s_kErrorInvalidPassword;
};

#endif  // REGISTERUSERCALL_H
