#include <QDebug>

#include "qremotefilesystemcontrollerimpl.h"
#include "qremotefilesystemcontroller.h"
#include "single.h"
#include "dependencyinjector.h"
#include "userservice.h"
#include "fileservice.h"
#include "qheliosfile.h"
#include "typeconversions.h"
#include "file.h"

QRemoteFileSystemControllerImpl::QRemoteFileSystemControllerImpl(QRemoteFileSystemController* publicImpl)
    : m_publicImpl(publicImpl)
{
    m_fileService = Single<DependencyInjector>::instance().getInstance<FileService>();
    if (!m_fileService)
    {
        qFatal("File service instance not available");
    }
}

void QRemoteFileSystemControllerImpl::registerForNotifications()
{
    m_fileService->registerListener(shared_from_this());
}

void QRemoteFileSystemControllerImpl::unregisterFromNotifications()
{
    m_fileService->unregisterListener(shared_from_this());
}

void QRemoteFileSystemControllerImpl::setAuthenticationToken(const QString& newVal)
{
    if (newVal.length() > 0)
    {
        m_fileService->setAuthToken(newVal.toStdString());
    }
}

void QRemoteFileSystemControllerImpl::resetAuthenticationToken()
{
    m_fileService->removeAuthToken();
}

QString QRemoteFileSystemControllerImpl::cwd() const
{
    return QString::fromStdString(m_fileService->currentDirectory());
}

QVariantList QRemoteFileSystemControllerImpl::files() const
{
    return m_files;
}

QVariantList QRemoteFileSystemControllerImpl::transfers() const
{
    return m_transfers;
}

void QRemoteFileSystemControllerImpl::openDirectory(const QString& dirName)
{
    m_fileService->changeCurrentDirectory(dirName.toStdString(), true);
}

void QRemoteFileSystemControllerImpl::currentDirectoryChanged()
{
    auto files = m_fileService->files();
    m_files.clear();
    m_files.reserve(safe_integral_cast<int>(files.size()));
    for (const auto& file : files)
    {
        m_files.push_back(QVariant::fromValue(QHeliosFile(file)));
    }
    QMetaObject::invokeMethod(m_publicImpl, "cwdChanged", Qt::QueuedConnection);
    QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
}

void QRemoteFileSystemControllerImpl::directoryCreated(std::shared_ptr<const File> directory)
{
    if (directory->parentDirectory() == m_fileService->currentDirectory())
    {
        m_files.push_back(QVariant::fromValue(QHeliosFile(directory)));
        QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "directoryCreatedInCwd",
                                  Q_ARG(const QVariant&, m_files.at(m_files.length() - 1)));
    }
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
