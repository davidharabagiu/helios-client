#ifndef SHAREKEYCALL_H
#define SHAREKEYCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class ShareKeyCall
 * @brief Manages a file key send call
 */
class ShareKeyCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param authToken - Authentication token
     * @param username - Name of the user which will receive the key
     * @param keyName - Name of the key
     * @param keyLength - Length of the key in bytes before it was encrypted
     * @param keyContent - Encrypted content of the key
     * @param callback - ShareKeyCallback
     */
    ShareKeyCall(const std::string& authToken, const std::string& username, const std::string& keyName,
                 const std::string& keyLength, const std::vector<uint8_t>& keyContent,
                 const ApiCallbacks::ShareKeyCallback& callback);

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
    ApiCallbacks::ShareKeyCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief Destination user name parameter name
     */
    static const std::string s_kUsernameParam;

    /**
     * @brief Key name parameter name
     */
    static const std::string s_kKeyNameParam;

    /**
     * @brief Key length parameter name
     */
    static const std::string s_kKeyLengthParam;

    /**
     * @brief Key content parameter name
     */
    static const std::string s_kKeyContentParam;

    /**
     * @brief Invalid user error message
     */
    static const std::string s_kErrorInvalidUser;
};

#endif  // SHAREKEYCALL_H
