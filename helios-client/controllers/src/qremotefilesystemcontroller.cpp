#include "qremotefilesystemcontroller.h"
#include "qremotefilesystemcontrollerimpl.h"

QRemoteFileSystemController::QRemoteFileSystemController(QObject* parent)
    : QObject(parent)
    , m_privateImpl(new QRemoteFileSystemControllerImpl(this))
{
    m_privateImpl->registerForNotifications();
}

QRemoteFileSystemController::~QRemoteFileSystemController()
{
    m_privateImpl->unregisterFromNotifications();
}

void QRemoteFileSystemController::setAuthenticationToken(const QString& newValue)
{
    m_privateImpl->setAuthenticationToken(newValue);
}

void QRemoteFileSystemController::resetAuthenticationToken()
{
    m_privateImpl->resetAuthenticationToken();
}

QString QRemoteFileSystemController::cwd() const
{
    return m_privateImpl->cwd();
}

QVariantList QRemoteFileSystemController::files() const
{
    return m_privateImpl->files();
}

QVariantList QRemoteFileSystemController::transfers() const
{
    return m_privateImpl->transfers();
}

void QRemoteFileSystemController::openDirectory(const QString& dirName)
{
    m_privateImpl->openDirectory(dirName);
}
