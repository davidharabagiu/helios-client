#ifndef DOWNLOADCALL_H
#define DOWNLOADCALL_H

#include <string>
#include <vector>
#include <cstdint>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class DownloadCall
 * @brief Manages a file download call
 */
class DownloadCall : public ApiCall
{
public:
    DownloadCall(const std::string& authToken, const std::string& transferId, uint64_t offset, const uint64_t& size,
                 const ApiCallbacks::DownloadCallback& callback);

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
    ApiCallbacks::DownloadCallback m_callback;

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
     * @brief Data chunk size parameter name
     */
    static const std::string s_kChunkSizeParam;

    /**
     * @brief Invalid transfer id error message
     */
    static const std::string s_kErrorInvalidTransferId;
};

#endif  // DOWNLOADCALL_H
