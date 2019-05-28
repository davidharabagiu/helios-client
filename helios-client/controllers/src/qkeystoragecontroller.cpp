#include "qkeystoragecontroller.h"

QKeyStorageController::QKeyStorageController(QObject* parent)
    : QObject(parent)
    , m_privateImpl(new QKeyStorageControllerImpl(this))
{
    m_privateImpl->registerForNotifications();
}

QKeyStorageController::~QKeyStorageController()
{
    m_privateImpl->unregisterFromNotifications();
}

void QKeyStorageController::setSession(const QUserSession& newValue)
{
    m_privateImpl->setSession(newValue);
}

void QKeyStorageController::resetSession()
{
    m_privateImpl->resetSession();
}

QStringList QKeyStorageController::keys(KeySize keySize) const
{
    return m_privateImpl->keys(static_cast<QKeyStorageControllerImpl::KeySize>(keySize));
}

bool QKeyStorageController::createKey(const QString& name, KeySize size)
{
    return m_privateImpl->createKey(name, static_cast<QKeyStorageControllerImpl::KeySize>(size));
}

bool QKeyStorageController::removeKey(const QString& name)
{
    return m_privateImpl->removeKey(name);
}

void QKeyStorageController::removeAllKeys()
{
    m_privateImpl->removeAllKeys();
}

void QKeyStorageController::sendKey(const QString& username, const QString& keyName)
{
    m_privateImpl->sendKey(username, keyName);
}
