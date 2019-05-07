#ifndef CHECKTOKENCALL_H
#define CHECKTOKENCALL_H

#include <string>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class CheckTokenCall
 * @brief Manages a check token call
 */
class CheckTokenCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param username - Username parameter
     * @param token - Authentication token
     * @param callback - Callback
     */
    CheckTokenCall(const std::string& username, const std::string& token,
                   const ApiCallbacks::CheckTokenCallback& callback);

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
     * @brief Http request object
     */
    std::shared_ptr<UrlEncodedRequest> m_request;

    /**
     * @brief Callback
     */
    ApiCallbacks::CheckTokenCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Username parameter name
     */
    static const std::string s_kUsernameParam;

    /**
     * @brief Token header parameter name
     */
    static const std::string s_kTokenParam;
};

#endif  // CHECKTOKENCALL_H
