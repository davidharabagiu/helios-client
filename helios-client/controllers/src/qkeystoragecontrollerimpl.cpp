#include <cassert>

#include "qkeystoragecontrollerimpl.h"
#include "single.h"
#include "dependencyinjector.h"
#include "keymanager.h"
#include "typeconversions.h"

QKeyStorageControllerImpl::QKeyStorageControllerImpl()
{
    m_keyManager = Single<DependencyInjector>::instance().getInstance<KeyManager>();
    if (!m_keyManager)
    {
        qFatal("KeyManager instance not available");
    }
}

QList<QString> QKeyStorageControllerImpl::keys(QKeyStorageControllerImpl::KeySize keySize) const
{
    QList<QString> result;

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
    return m_keyManager->createKey(name.toStdString(), keySizeToByteLength(size));
}

bool QKeyStorageControllerImpl::removeKey(const QString& name)
{
    return m_keyManager->removeKey(name.toStdString());
}

void QKeyStorageControllerImpl::removeAllKeys()
{
    m_keyManager->removeAllKeys();
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
