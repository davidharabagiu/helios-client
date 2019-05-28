#ifndef ACCEPTFILECALL_H
#define ACCEPTFILECALL_H

#include <string>
#include <vector>
#include <QString>

#include "apicall.h"
#include "apicalldefs.h"
#include "urlencodedrequest.h"

/**
 * @class AcceptFileCall
 * @brief Manages a accept file share accept call
 */
class AcceptFileCall : public ApiCall
{
public:
    /**
     * @brief Constructor
     * @param authToken - Authentication token
     * @param notificationId - Id of the notification carrying the file information
     * @param path - New path of the shared file in the current user's storage
     * @param callback - AcceptFileCallback
     */
    AcceptFileCall(const std::string& authToken, const std::string& notificationId, const std::string& path,
                   const ApiCallbacks::AcceptFileCallback& callback);

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
    ApiCallbacks::AcceptFileCallback m_callback;

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
     * @brief File path parameter name
     */
    static const std::string s_kPathParam;

    /**
     * @brief Invalid notification type error message
     */
    static const std::string s_kErrorInvalidNotificationType;

    /**
     * @brief Source file no longer exists error message
     */
    static const std::string s_kErrorFileNoLongerExists;

    /**
     * @brief Destination file already exists error message
     */
    static const std::string s_kErrorFileAlreadyExists;

    /**
     * @brief Invalid path error message
     */
    static const std::string s_kErrorInvalidPath;
};

#endif  // ACCEPTFILECALL_H
