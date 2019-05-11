#include <QDebug>
#include <algorithm>
#include <cassert>

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

void QRemoteFileSystemControllerImpl::setCwd(const QString& newValue)
{
    m_fileService->changeCurrentDirectory(newValue.toStdString(), false);
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

void QRemoteFileSystemControllerImpl::goBack()
{
    m_fileService->navigateBack();
}

void QRemoteFileSystemControllerImpl::createDirectory(const QString& dirName)
{
    if (dirName.length() > 0)
    {
        m_fileService->createDirectory(dirName.toStdString(), true);
    }
}

void QRemoteFileSystemControllerImpl::remove(const QString& fileName)
{
    if (fileName.length() > 0)
    {
        m_fileService->removeFile(fileName.toStdString(), true);
    }
}

void QRemoteFileSystemControllerImpl::move(const QString& fileName, const QString& destinationDirPath)
{
    if (fileName.length() > 0)
    {
        m_fileService->moveFile((m_fileService->currentDirectory() == "") ?
                                    fileName.toStdString() :
                                    (m_fileService->currentDirectory() + "/" + fileName.toStdString()),
                                (destinationDirPath.toStdString() == "") ?
                                    fileName.toStdString() :
                                    (destinationDirPath.toStdString() + "/" + fileName.toStdString()));
    }
}

void QRemoteFileSystemControllerImpl::rename(const QString& fileName, const QString& newName)
{
    if (fileName.length() > 0 && newName.length() > 0)
    {
        m_fileService->moveFile((m_fileService->currentDirectory() == "") ?
                                    fileName.toStdString() :
                                    (m_fileService->currentDirectory() + "/" + fileName.toStdString()),
                                (m_fileService->currentDirectory() == "") ?
                                    newName.toStdString() :
                                    (m_fileService->currentDirectory() + "/" + newName.toStdString()));
    }
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
        QHeliosFile newDirectory(directory);
        m_files.push_back(QVariant::fromValue(newDirectory));

        QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "directoryCreatedInCwd", Qt::QueuedConnection,
                                  Q_ARG(const QHeliosFile&, newDirectory));
    }
}

void QRemoteFileSystemControllerImpl::fileMoved(std::shared_ptr<const File> oldFile, std::shared_ptr<const File> file)
{
    if (oldFile->parentDirectory() == m_fileService->currentDirectory())
    {
        auto it = std::find_if(m_files.begin(), m_files.end(), [&oldFile](const QVariant& el) {
            return qvariant_cast<QHeliosFile>(el).name().toStdString() == oldFile->name();
        });

        assert(it != m_files.end());
        if (it != m_files.end())
        {
            m_files.erase(it);
        }

        QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "fileRemovedFromCwd", Qt::QueuedConnection,
                                  Q_ARG(const QString&, QString::fromStdString(oldFile->name())));
    }
    if (file->parentDirectory() == m_fileService->currentDirectory())
    {
        QHeliosFile newFile(file);
        m_files.push_back(QVariant::fromValue(newFile));

        QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "directoryCreatedInCwd", Qt::QueuedConnection,
                                  Q_ARG(const QHeliosFile&, newFile));
    }
}

void QRemoteFileSystemControllerImpl::fileRemoved(std::shared_ptr<const File> file)
{
    if (file->parentDirectory() == m_fileService->currentDirectory())
    {
        auto it = std::find_if(m_files.begin(), m_files.end(), [&file](const QVariant& el) {
            return qvariant_cast<QHeliosFile>(el).name().toStdString() == file->name();
        });

        assert(it != m_files.end());
        if (it != m_files.end())
        {
            m_files.erase(it);
        }

        QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "fileRemovedFromCwd", Qt::QueuedConnection,
                                  Q_ARG(const QString&, QString::fromStdString(file->name())));
    }
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

void QRemoteFileSystemControllerImpl::errorOccured(const std::string& errorString)
{
    QMetaObject::invokeMethod(m_publicImpl, "error", Qt::QueuedConnection,
                              Q_ARG(const QString&, QString::fromStdString(errorString)));
}
