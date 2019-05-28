#include <cassert>

#include "qkeystoragecontrollerimpl.h"
#include "qkeystoragecontroller.h"
#include "single.h"
#include "dependencyinjector.h"
#include "keymanager.h"
#include "typeconversions.h"
#include "keyexchangeservice.h"

QKeyStorageControllerImpl::QKeyStorageControllerImpl(QKeyStorageController* publicImpl)
    : m_publicImpl(publicImpl)
{
    m_keyManager = Single<DependencyInjector>::instance().getInstance<KeyManager>();
    if (!m_keyManager)
    {
        qFatal("KeyManager instance not available");
    }

    m_keyExchangeService = Single<DependencyInjector>::instance().getInstance<KeyExchangeService>();
    if (!m_keyExchangeService)
    {
        qFatal("KeyExchangeService instance not available");
    }
}

void QKeyStorageControllerImpl::registerForNotifications()
{
    m_keyExchangeService->registerListener(shared_from_this());
}

void QKeyStorageControllerImpl::unregisterFromNotifications()
{
    m_keyExchangeService->unregisterListener(shared_from_this());
}

QStringList QKeyStorageControllerImpl::keys(QKeyStorageControllerImpl::KeySize keySize) const
{
    QStringList result;

    auto keys = m_keyManager->listKeys(keySizeToByteLength(keySize));
    result.reserve(safe_integral_cast<int>(keys.size()));

    for (const auto& name : keys)
    {
        result.push_back(QString::fromStdString(name));
    }

    return result;
}

bool QKeyStorageControllerImpl::createKey(const QString& name, QKeyStorageControllerImpl::KeySize size)
{
    bool result = m_keyManager->createKey(name.toStdString(), keySizeToByteLength(size));
    emit m_publicImpl->keysChanged();
    return result;
}

bool QKeyStorageControllerImpl::removeKey(const QString& name)
{
    bool result = m_keyManager->removeKey(name.toStdString());
    emit m_publicImpl->keysChanged();
    return result;
}

void QKeyStorageControllerImpl::removeAllKeys()
{
    m_keyManager->removeAllKeys();
    emit m_publicImpl->keysChanged();
}

void QKeyStorageControllerImpl::sendKey(const QString& username, const QString& keyName)
{
    m_keyExchangeService->sendKey(username.toStdString(), keyName.toStdString());
}

void QKeyStorageControllerImpl::keySharedSuccessfully()
{
    QMetaObject::invokeMethod(m_publicImpl, "keyShareResult", Qt::QueuedConnection, Q_ARG(bool, true),
                              Q_ARG(QString, "Key shared successfully"));
}

void QKeyStorageControllerImpl::errorOccured(KeyExchangeServiceListener::Error error)
{
    QString message;
    switch (error)
    {
        case KeyExchangeServiceListener::Error::NO_SUCH_KEY:
        {
            message = "Requested key was not found";
            break;
        }
        case KeyExchangeServiceListener::Error::UNKNOWN_USER:
        {
            message = "Key recipient was not found";
            break;
        }
        case KeyExchangeServiceListener::Error::RECIPIENT_DISABLED_TRANSFERS:
        {
            message = "The key recipient doesn't allow transfers at this time";
            break;
        }
        default:
        {
            message = "An unknown error occured while sharing the key";
            break;
        }
    }
    QMetaObject::invokeMethod(m_publicImpl, "keyShareResult", Qt::QueuedConnection, Q_ARG(bool, false),
                              Q_ARG(QString, message));
}

uint16_t QKeyStorageControllerImpl::keySizeToByteLength(QKeyStorageControllerImpl::KeySize keySize)
{
    switch (keySize)
    {
        case KeySize::_128:
        {
            return 16;
        }
        case KeySize::_192:
        {
            return 24;
        }
        case KeySize::_256:
        {
            return 32;
        }
    }

    assert(false);
    return 0;
}
