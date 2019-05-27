#include <cassert>

#include "qkeystoragecontrollerimpl.h"
#include "qkeystoragecontroller.h"
#include "single.h"
#include "dependencyinjector.h"
#include "keymanager.h"
#include "typeconversions.h"

QKeyStorageControllerImpl::QKeyStorageControllerImpl(QKeyStorageController* publicImpl)
    : m_publicImpl(publicImpl)
{
    m_keyManager = Single<DependencyInjector>::instance().getInstance<KeyManager>();
    if (!m_keyManager)
    {
        qFatal("KeyManager instance not available");
    }
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

bool QKeyStorageControllerImpl::sendKey(const QString& username, const QString& keyName)
{
    return m_keyManager->sendKey(username.toStdString(), keyName.toStdString());
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
