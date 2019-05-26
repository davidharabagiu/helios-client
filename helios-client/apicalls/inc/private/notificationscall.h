#ifndef NOTIFICATIONSCALL_H
#define NOTIFICATIONSCALL_H

#include <string>
#include <vector>
#include <QString>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class NotificationsCall
 * @brief Manages a notifications listing call
 */
class NotificationsCall : public ApiCall
{
public:
    NotificationsCall(const std::string& authToken, const ApiCallbacks::NotificationsCallback& callback);

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
    ApiCallbacks::NotificationsCallback m_callback;

    /**
     * @brief Request url
     */
    static const std::string s_kUrl;

    /**
     * @brief Authentication token header parameter name
     */
    static const std::string s_kTokenParam;

    /**
     * @brief Name of the JSON field representing the notification id in the reply
     */
    static const QString s_kIdJsonField;

    /**
     * @brief Name of the JSON field representing the notification text in the reply
     */
    static const QString s_kTextJsonField;

    /**
     * @brief Name of the JSON field representing the notification type in the reply
     */
    static const QString s_kTypeJsonField;

    /**
     * @brief Name of the JSON value of the "type" field that marks the notification as being a "file shared"
     * notification
     */
    static const QString s_kFileShareJsonValue;

    /**
     * @brief Name of the JSON value of the "type" field that marks the notification as being a "key shared"
     * notification
     */
    static const QString s_kKeyShareJsonValue;
};

#endif  // NOTIFICATIONSCALL_H
