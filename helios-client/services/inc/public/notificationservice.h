#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <vector>

#include "observable.h"
#include "notificationservicelistener.h"
#include "notification.h"
#include "authenticatedserviceinterface.h"

/**
 * @class NotificationService
 * @brief Notification management service
 */
class NotificationService : public AuthenticatedServiceInterface,
                            public Observable<NotificationServiceListener, ObservableNotifyMode::ASYNC>
{
public:
    /**
     * @brief Manually refresh the notifications list
     */
    virtual void refresh() = 0;

    /**
     * @brief Returns a list with all available notifications
     * @return std::vector<Notification>
     */
    virtual std::vector<Notification> notifications() const = 0;

    /**
     * @brief Dismiss a notification with the given id
     * @param notificationId - Notification id
     */
    virtual void dismissNotification(const std::string& notificationId) = 0;

    /**
     * @brief Dismiss all notifications
     */
    virtual void dismissAllNotifications() = 0;
};

#endif  // NOTIFICATIONSERVICE_H
