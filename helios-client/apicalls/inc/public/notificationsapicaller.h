#ifndef NOTIFICATIONSAPICALLER_H
#define NOTIFICATIONSAPICALLER_H

#include <string>
#include <functional>

#include "apicalldefs.h"

/**
 * @class NotificationsApiCaller
 * @brief Provides calls for the notifications management API
 */
class NotificationsApiCaller
{
public:
    /**
     * @brief Destructor
     */
    virtual ~NotificationsApiCaller() = default;

    /**
     * @brief Get all the notifications of the current user
     * @param authToken - Authentication token
     * @param callback - NotificationsCallback
     */
    virtual void notifications(const std::string&                         authToken,
                               const ApiCallbacks::NotificationsCallback& callback) const = 0;

    /**
     * @brief Dismiss a notification with the given id
     * @param authToken - Authentication token
     * @param notificationId - Notification id
     * @param callback - DismissNotificationCallback
     */
    virtual void dismissNotification(const std::string& authToken, const std::string& notificationId,
                                     const ApiCallbacks::DismissNotificationCallback& callback) const = 0;

    /**
     * @brief Dismiss all notifications
     * @param authToken - Authentication token
     * @param callback - DismissAllNotificationsCallback
     */
    virtual void dismissAllNotifications(const std::string&                                   authToken,
                                         const ApiCallbacks::DismissAllNotificationsCallback& callback) const = 0;
};

#endif  // NOTIFICATIONSAPICALLER_H
