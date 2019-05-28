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

void QNotificationsControllerImpl::registerForNotifications()
{
    m_service->registerListener(shared_from_this());
}

void QNotificationsControllerImpl::unregisterFromNotifications()
{
    m_service->unregisterListener(shared_from_this());
}

void QNotificationsControllerImpl::setSession(const QUserSession& newVal)
{
    if (newVal.isValid())
    {
        m_service->setSession(*newVal.data());
    }
    else
    {
        resetSession();
    }
}

void QNotificationsControllerImpl::resetSession()
{
    m_service->removeSession();
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
        newNotifications.push_back(QVariant::fromValue(QNotification(std::make_shared<Notification>(el))));
    }

    m_notifications = newNotifications;
    QMetaObject::invokeMethod(m_publicImpl, "notificationsChanged", Qt::QueuedConnection);
}
