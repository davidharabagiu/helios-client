#include <QDebug>

#include "qnotificationscontroller.h"
#include "qnotificationscontrollerimpl.h"
#include "single.h"
#include "dependencyinjector.h"
#include "notificationservice.h"
#include "qnotification.h"

QNotificationsControllerImpl::QNotificationsControllerImpl(QNotificationsController* publicImpl)
    : m_publicImpl(publicImpl)
{
    m_service = Single<DependencyInjector>::instance().getInstance<NotificationService>();
    if (!m_service)
    {
        qFatal("NotificationService instance not available");
    }
}

QNotificationsControllerImpl::~QNotificationsControllerImpl()
{
    unregisterFromNotifications();
}

void QNotificationsControllerImpl::registerForNotifications()
{
    m_service->registerListener(shared_from_this());
}

void QNotificationsControllerImpl::unregisterFromNotifications()
{
    m_service->unregisterListener(shared_from_this());
}

void QNotificationsControllerImpl::setAuthenticationToken(const QString& newValue)
{
    m_service->setAuthToken(newValue.toStdString());
}

void QNotificationsControllerImpl::resetAuthenticationToken()
{
    m_service->removeAuthToken();
}

QVariantList QNotificationsControllerImpl::notifications() const
{
    return m_notifications;
}

void QNotificationsControllerImpl::dismissNotification(const QString& notificationId)
{
    m_service->dismissNotification(notificationId.toStdString());
}

void QNotificationsControllerImpl::dismissAllNotifications()
{
    m_service->dismissAllNotifications();
}

void QNotificationsControllerImpl::notificationListUpdated()
{
    QVariantList newNotifications;
    auto         serviceNotifications = m_service->notifications();
    for (const auto& el : serviceNotifications)
    {
        m_notifications.push_back(QVariant::fromValue(QNotification(std::make_shared<Notification>(el))));
    }

    m_notifications = newNotifications;
    QMetaObject::invokeMethod(m_publicImpl, "notificationsChanged", Qt::QueuedConnection);
}
