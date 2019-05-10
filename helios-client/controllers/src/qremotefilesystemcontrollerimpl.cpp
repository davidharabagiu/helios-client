#include <QDebug>

#include "qremotefilesystemcontrollerimpl.h"
#include "qremotefilesystemcontroller.h"
#include "single.h"
#include "dependencyinjector.h"
#include "userservice.h"
#include "fileservice.h"

QRemoteFileSystemControllerImpl::QRemoteFileSystemControllerImpl(QRemoteFileSystemController* publicImpl)
    : m_publicImpl(publicImpl)
{
    m_fileService = Single<DependencyInjector>::instance().getInstance<FileService>();
    if (!m_fileService)
    {
        qFatal("File service instance not available");
    }

    (void)m_publicImpl;
}

void QRemoteFileSystemControllerImpl::registerForNotifications()
{
    m_fileService->registerListener(shared_from_this());
}

void QRemoteFileSystemControllerImpl::unregisterFromNotifications()
{
    m_fileService->unregisterListener(shared_from_this());
}

void QRemoteFileSystemControllerImpl::fileServiceEnabled()
{
}

void QRemoteFileSystemControllerImpl::currentDirectoryChanged()
{
}

void QRemoteFileSystemControllerImpl::directoryCreated(const std::string& /*path*/)
{
}

void QRemoteFileSystemControllerImpl::fileMoved(const std::string& /*sourcePath*/,
                                                const std::string& /*destinationPath*/)
{
}

void QRemoteFileSystemControllerImpl::fileRemoved(const std::string& /*path*/)
{
}

void QRemoteFileSystemControllerImpl::fileDownloadStarted(const std::string& /*path*/)
{
}

void QRemoteFileSystemControllerImpl::fileUploadStarted(const std::string& /*path*/)
{
}

void QRemoteFileSystemControllerImpl::fileOperationProgressChanged(const std::string& /*path*/)
{
}

void QRemoteFileSystemControllerImpl::fileOperationCompleted(const std::string& /*path*/)
{
}

void QRemoteFileSystemControllerImpl::fileOperationAborted(const std::string& /*path*/)
{
}

void QRemoteFileSystemControllerImpl::errorOccured(const std::string& /*errorString*/)
{
}
