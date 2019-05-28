#ifndef ACCEPTKEYCALL_H
#define ACCEPTKEYCALL_H

#include <string>
#include <vector>
#include <QString>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class AcceptKeyCall
 * @brief Manages a accept file key send call
 */
class AcceptKeyCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param authToken - Authentication token
     * @param notificationId - Id of the notification carrying the key information
     * @param callback - AcceptKeyCallback
     */
    AcceptKeyCall(const std::string& authToken, const std::string& notificationId,
                  const ApiCallbacks::AcceptKeyCallback& callback);

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
    ApiCallbacks::AcceptKeyCallback m_callback;

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

    /**
     * @brief Name of the JSON field representing the key name in the reply
     */
    static const QString s_kKeyNameJsonField;

    /**
     * @brief Name of the JSON field representing the key length in the reply
     */
    static const QString s_kKeyLengthJsonField;

    /**
     * @brief Name of the JSON field representing the key content in the reply
     */
    static const QString s_kKeyContentJsonField;

    /**
     * @brief Invalid notification type error message
     */
    static const std::string s_kErrorInvalidNotificationType;
};

#endif  // ACCEPTKEYCALL_H
