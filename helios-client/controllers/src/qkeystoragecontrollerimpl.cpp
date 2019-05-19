#include "qkeystoragecontrollerimpl.h"

QKeyStorageControllerImpl::QKeyStorageControllerImpl()
{
}

QList<QString> QKeyStorageControllerImpl::keys(QKeyStorageControllerImpl::KeySize /*keySize*/) const
{
    return {};
}

bool QKeyStorageControllerImpl::createKey(const QString& /*name*/, QKeyStorageControllerImpl::KeySize /*size*/)
{
    return false;
}

bool QKeyStorageControllerImpl::removeKey(const QString& /*name*/)
{
    return false;
}

void QKeyStorageControllerImpl::removeAllKeys()
{
}
