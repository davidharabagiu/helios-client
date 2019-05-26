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

void QNotificationsController::setAuthenticationToken(const QString& newValue)
{
    m_privateImpl->setAuthenticationToken(newValue);
}

void QNotificationsController::resetAuthenticationToken()
{
    m_privateImpl->resetAuthenticationToken();
}

QVariantList QNotificationsController::notifications() const
{
    return m_privateImpl->notifications();
}
