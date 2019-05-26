#ifndef DISMISSNOTIFICATIONCALL_H
#define DISMISSNOTIFICATIONCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class DismissNotificationCall
 * @brief Manages a dismiss notification call
 */
class DismissNotificationCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param authToken - Authentication token
     * @param notificationId - Notification id
     * @param callback - Callback function
     */
    DismissNotificationCall(const std::string& authToken, const std::string& notificationId,
                            const ApiCallbacks::DismissNotificationCallback& callback);

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
    ApiCallbacks::DismissNotificationCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief Notification id parameter name
     */
    static const std::string s_kNotificationIdParam;
};

#endif  // DISMISSNOTIFICATIONCALL_H
