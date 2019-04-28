#ifndef CREATEDIRECTORYCALL_H
#define CREATEDIRECTORYCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class CreateDirectoryCall
 * @brief Manages a directory creation call
 */
class CreateDirectoryCall : public ApiCall
{
public:
    CreateDirectoryCall(const std::string& authToken, const std::string& path,
                        const ApiCallbacks::CreateDirectoryCallback& callback);

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
    ApiCallbacks::CreateDirectoryCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief Directory path parameter name
     */
    static const std::string s_kPathParam;

    /**
     * @brief Directory already exists error message
     */
    static const std::string s_kErrorDirectoryExists;

    /**
     * @brief Invalid path error message
     */
    static const std::string s_kErrorInvalidPath;
};

#endif  // CREATEDIRECTORYCALL_H
