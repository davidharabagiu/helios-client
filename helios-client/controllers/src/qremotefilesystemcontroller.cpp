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
