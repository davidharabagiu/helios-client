#ifndef QNOTIFICATIONSCONTROLLER_H
#define QNOTIFICATIONSCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <memory>

#include "qusersession.h"
#include "qnotification.h"

// Forward declarations
class QNotificationsControllerImpl;

/**
 * @class QNotificationsController
 * @brief Controller for notification operations (query, dismiss)
 */
class QNotificationsController : public QObject
{
    Q_OBJECT

    /**
     * @property authenticationToken
     * @brief Current user session used to access the user's remote files
     */
    Q_PROPERTY(QUserSession session WRITE setSession RESET resetSession)

    /**
     * @property notifications
     * @brief List of current notifications. QVariantList of QNotification
     */
    Q_PROPERTY(QVariantList notifications READ notifications NOTIFY notificationsChanged)

public:
    Q_ENUMS(QNotification::QNotificationType);

    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit QNotificationsController(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~QNotificationsController();

    /**
     * @brief Setter for session
     * @param newValue - QString
     */
    void setSession(const QUserSession& newValue);

    /**
     * @brief Remove the current user session
     */
    void resetSession();

    /**
     * @brief Getter for notifications
     * @return QVariantList of QNotifications
     */
    QVariantList notifications() const;

    /**
     * @brief Dismiss notification by notification id
     * @param notificationId - notification id
     */
    Q_INVOKABLE void dismissNotification(const QString& notificationId);

    /**
     * @brief Dismiss all notifications
     */
    Q_INVOKABLE void dismissAllNotifications();

signals:
    /**
     * @brief SIGNAL emitted when notifications changes
     */
    void notificationsChanged();

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QNotificationsControllerImpl> m_privateImpl;
};

#endif  // QNOTIFICATIONSCONTROLLER_H
