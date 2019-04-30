#ifndef UPLOADCALL_H
#define UPLOADCALL_H

#include <string>
#include <vector>
#include <cstdint>

#include "apicall.h"
#include "apicalldefs.h"
#include "formdatarequest.h"

/**
 * @class UploadCall
 * @brief Manages a file upload call
 */
class UploadCall : public ApiCall
{
public:
    UploadCall(const std::string& authToken, const std::string& transferId, uint64_t offset,
               const std::vector<uint8_t>& content, const ApiCallbacks::UploadCallback& callback);

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
    ApiCallbacks::UploadCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief File transfer id parameter name
     */
    static const std::string s_kTranferIdParam;

    /**
     * @brief Offset parameter name
     */
    static const std::string s_kOffsetParam;

    /**
     * @brief File content parameter name
     */
    static const std::string s_kFileContentParam;

    /**
     * @brief Invalid transfer id error message
     */
    static const std::string s_kErrorInvalidTransferId;
};

#endif  // UPLOADCALL_H
