#ifndef QNOTIFICATIONSCONTROLLERIMPL_H
#define QNOTIFICATIONSCONTROLLERIMPL_H

#include <memory>
#include <QList>
#include <QVariant>

#include "notificationservicelistener.h"

// Forward declarations
class QNotificationsController;
class NotificationService;

/**
 * @class QNotificationsControllerImpl
 * @brief Private implementation of QNotificationsController
 */
class QNotificationsControllerImpl : public NotificationServiceListener,
                                     public std::enable_shared_from_this<QNotificationsControllerImpl>
{
public:
    /**
     * @brief Constructor
     * @param publicImpl - Public implementation
     */
    QNotificationsControllerImpl(QNotificationsController* publicImpl);

    /**
     * @brief Destructor
     */
    ~QNotificationsControllerImpl();

    /**
     * @brief Register for notifications from internal services
     */
    void registerForNotifications();

    /**
     * @brief Unregister from notifications from internal services
     */
    void unregisterFromNotifications();

public:  // Forwarded from QNotificationsController
    void         setAuthenticationToken(const QString& newValue);
    void         resetAuthenticationToken();
    QVariantList notifications() const;
    void         dismissNotification(const QString& notificationId);
    void         dismissAllNotifications();

public:  // From NotificationServiceListener
    void notificationListUpdated() override;

private:
    /**
     * @brief Public implementation
     */
    QNotificationsController* m_publicImpl;

    /**
     * @brief Notification service instance
     */
    std::shared_ptr<NotificationService> m_service;

    /**
     * @brief Cached user notifications
     */
    QVariantList m_notifications;
};

#endif  // QNOTIFICATIONSCONTROLLERIMPL_H
