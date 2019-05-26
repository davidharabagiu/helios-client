#ifndef DISMISSALLNOTIFICATIONSCALL_H
#define DISMISSALLNOTIFICATIONSCALL_H

#include <string>
#include <vector>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class DismissAllNotificationsCall
 * @brief Manages a dismiss all notifications call
 */
class DismissAllNotificationsCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param authToken - Authentication token
     * @param callback - Callback function
     */
    DismissAllNotificationsCall(const std::string&                                   authToken,
                                const ApiCallbacks::DismissAllNotificationsCallback& callback);

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
    ApiCallbacks::DismissAllNotificationsCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;
};

#endif  // DISMISSALLNOTIFICATIONSCALL_H
