#ifndef GETFILESIZECALL_H
#define GETFILESIZECALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class GetFileSizeCall
 * @brief Manages a file size query call
 */
class GetFileSizeCall : public ApiCall
{
public:
    GetFileSizeCall(const std::string& authToken, const std::string& path,
                    const ApiCallbacks::GetFileSizeCallback& callback);

public:
    /**
     * @brief Returns the http request
     * @return std::unique_ptr<UrlEncodedRequest>
     */
    std::unique_ptr<UrlEncodedRequest> request();

public:  // from ApiCall
    void send(std::shared_ptr<ApiCallVisitor> visitor) override;
    void receive(HttpStatus status, const std::vector<uint8_t>& reply) override;

private:
    /**
     * @brief Http request
     */
    std::unique_ptr<UrlEncodedRequest> m_request;

    /**
     * @brief Callback
     */
    ApiCallbacks::GetFileSizeCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief File path parameter name
     */
    static const std::string s_kPathParam;

    /**
     * @brief Invalid path error message
     */
    static const std::string s_kErrorInvalidPath;
};

#endif  // GETFILESIZECALL_H
