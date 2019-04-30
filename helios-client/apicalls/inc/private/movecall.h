#ifndef MOVECALL_H
#define MOVECALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class MoveCall
 * @brief Manages a file or directory move call
 */
class MoveCall : public ApiCall
{
public:
    MoveCall(const std::string& authToken, const std::string& path, const std::string& destination,
             const ApiCallbacks::MoveCallback& callback);

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
    ApiCallbacks::MoveCallback m_callback;

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
     * @brief Destination path parameter name
     */
    static const std::string s_kDestinationParam;

    /**
     * @brief Invalid source path error message
     */
    static const std::string s_kErrorInvalidSourcePath;

    /**
     * @brief Destination already exists error message
     */
    static const std::string s_kErrorDestinationExists;
};

#endif  // MOVECALL_H
