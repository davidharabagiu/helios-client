#ifndef ISDIRCALL_H
#define ISDIRCALL_H

#include <string>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class IsDirCall
 * @brief Manages a directory check call
 */
class IsDirCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param token - Authentication token
     * @param path - File or directory path
     * @param callback - Callback
     */
    IsDirCall(const std::string& token, const std::string& path, const ApiCallbacks::IsDirCallback& callback);

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
    ApiCallbacks::IsDirCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief Path parameter name
     */
    static const std::string s_kPathParam;
};

#endif  // ISDIRCALL_H
