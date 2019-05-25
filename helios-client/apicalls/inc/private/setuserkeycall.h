#ifndef SETUSERKEYCALL_H
#define SETUSERKEYCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "formdatarequest.h"

/**
 * @class SetUserKeyCall
 * @brief Manages a user public key set call
 */
class SetUserKeyCall : public ApiCall
{
public:
    SetUserKeyCall(const std::string& authToken, const std::string& key,
                   const ApiCallbacks::SetUserKeyCallback& callback);

public:
    /**
     * @brief Returns the http request
     * @return std::shared_ptr<FormDataRequest>
     */
    std::shared_ptr<FormDataRequest> request();

public:  // from ApiCall
    void send(std::shared_ptr<ApiCallVisitor> visitor) override;
    void receive(HttpStatus status, const std::vector<uint8_t>& reply) override;

private:
    /**
     * @brief Http request
     */
    std::shared_ptr<FormDataRequest> m_request;

    /**
     * @brief Callback
     */
    ApiCallbacks::SetUserKeyCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;
    /**
     * @brief Key parameter name
     */
    static const std::string s_kKeyParam;
};

#endif  // SETUSERKEYCALL_H
