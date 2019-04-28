#ifndef ENDTRANSFERCALL_H
#define ENDTRANSFERCALL_H

#include <string>
#include <vector>
#include <cstdint>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class EndTransferCall
 * @brief Manages a transfer end call
 */
class EndTransferCall : public ApiCall
{
public:
    EndTransferCall(const std::string& authToken, const std::string& transferId,
                    const ApiCallbacks::EndTransferCallback& callback);

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
    ApiCallbacks::EndTransferCallback m_callback;

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
     * @brief Invalid transfer id error message
     */
    static const std::string s_kErrorInvalidTransferId;
};

#endif  // DOWNLOADCALL_H
