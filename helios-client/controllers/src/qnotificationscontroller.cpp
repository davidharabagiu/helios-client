#include "qnotificationscontroller.h"
#include "qnotificationscontrollerimpl.h"

QNotificationsController::QNotificationsController(QObject* parent)
    : QObject(parent)
    , m_privateImpl(new QNotificationsControllerImpl(this))
{
    m_privateImpl->registerForNotifications();
}

QNotificationsController::~QNotificationsController()
{
    m_privateImpl->unregisterFromNotifications();
}

void QNotificationsController::setSession(const QUserSession& newValue)
{
    m_privateImpl->setSession(newValue);
}

void QNotificationsController::resetSession()
{
    m_privateImpl->resetSession();
}

QVariantList QNotificationsController::notifications() const
{
    return m_privateImpl->notifications();
}

void QNotificationsController::dismissNotification(const QString& notificationId)
{
    m_privateImpl->dismissNotification(notificationId);
}

void QNotificationsController::dismissAllNotifications()
{
    m_privateImpl->dismissAllNotifications();
}
