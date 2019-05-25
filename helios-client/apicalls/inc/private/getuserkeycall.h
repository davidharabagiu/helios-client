#ifndef GETUSERKEYCALL_H
#define GETUSERKEYCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class GetUserKeyCall
 * @brief Manages a user public key query call
 */
class GetUserKeyCall : public ApiCall
{
public:
    GetUserKeyCall(const std::string& authToken, const std::string& username,
                   const ApiCallbacks::GetUserKeyCallback& callback);

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
    ApiCallbacks::GetUserKeyCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief Username parameter name
     */
    static const std::string s_kUsernameParam;

    /**
     * @brief Invalid username error message
     */
    static const std::string s_kErrorInvalidUser;

    /**
     * @brief No key specified error message
     */
    static const std::string s_kErrorNoKey;
};

#endif  // GETUSERKEYCALL_H
