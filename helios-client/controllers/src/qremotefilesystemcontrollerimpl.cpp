#include <QDebug>
#include <QDir>
#include <algorithm>
#include <cassert>

#include "qremotefilesystemcontrollerimpl.h"
#include "qremotefilesystemcontroller.h"
#include "single.h"
#include "dependencyinjector.h"
#include "userservice.h"
#include "fileservice.h"
#include "qheliosfile.h"
#include "qfiletransfer.h"
#include "typeconversions.h"
#include "file.h"
#include "pathutils.h"

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

void QRemoteFileSystemControllerImpl::upload(const QUrl& localPath, const QString& encryptionKeyName)
{
    const auto& _localPath = localPath.url(QUrl::PreferLocalFile);
    if (!_localPath.isEmpty())
    {
        std::string fileName;
        std::string filePath;
        PathUtils::getFileNameAndParentDir(_localPath.toStdString(), fileName, filePath);
        m_fileService->uploadFile(_localPath.toStdString(), fileName, true, encryptionKeyName.toStdString());
    }
}

void QRemoteFileSystemControllerImpl::download(const QString& filename, const QUrl& destinationDir,
                                               const QString& decryptionKeyName)
{
    if (!filename.isEmpty())
    {
        const auto& localPath = QDir(destinationDir.url(QUrl::PreferLocalFile)).filePath(filename);
        if (!localPath.isEmpty())
        {
            m_fileService->downloadFile(filename.toStdString(), true, localPath.toStdString(),
                                        decryptionKeyName.toStdString());
        }
    }
}

void QRemoteFileSystemControllerImpl::cancelTransfer(const QFileTransfer& transfer)
{
    m_fileService->cancelOperation(transfer.remotePath().toStdString());
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
        QMetaObject::invokeMethod(m_publicImpl, "fileAddedInCwd", Qt::QueuedConnection,
                                  Q_ARG(QHeliosFile, newDirectory));
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
            QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
            QMetaObject::invokeMethod(m_publicImpl, "fileRemovedFromCwd", Qt::QueuedConnection,
                                      Q_ARG(QString, QString::fromStdString(oldFile->name())));
        }
    }
    if (file->parentDirectory() == m_fileService->currentDirectory())
    {
        QHeliosFile newFile(file);
        m_files.push_back(QVariant::fromValue(newFile));

        QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "fileAddedInCwd", Qt::QueuedConnection, Q_ARG(QHeliosFile, newFile));
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
            QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
            QMetaObject::invokeMethod(m_publicImpl, "fileRemovedFromCwd", Qt::QueuedConnection,
                                      Q_ARG(QString, QString::fromStdString(file->name())));
        }
    }
}

void QRemoteFileSystemControllerImpl::uploadedFileInCurrentDir(std::shared_ptr<const File> file)
{
    if (file->parentDirectory() == m_fileService->currentDirectory())
    {
        auto newFile = QHeliosFile(file);

        auto it = std::find_if(m_files.begin(), m_files.end(), [&file](const QVariant& el) {
            return qvariant_cast<QHeliosFile>(el).name().toStdString() == file->name();
        });

        if (it != m_files.end())
        {
            QMetaObject::invokeMethod(m_publicImpl, "fileRemovedFromCwd", Qt::QueuedConnection,
                                      Q_ARG(QString, QString::fromStdString(file->name())));
            m_files[safe_integral_cast<int>(std::distance(m_files.begin(), it))] = QVariant::fromValue(newFile);
        }
        else
        {
            m_files.push_back(QVariant::fromValue(newFile));
        }

        QMetaObject::invokeMethod(m_publicImpl, "fileAddedInCwd", Qt::QueuedConnection, Q_ARG(QHeliosFile, newFile));
        QMetaObject::invokeMethod(m_publicImpl, "filesChanged", Qt::QueuedConnection);
    }
}

void QRemoteFileSystemControllerImpl::transferStarted(std::shared_ptr<FileTransfer> transfer)
{
    QFileTransfer newTransfer(transfer);
    m_transfers.push_back(QVariant::fromValue(newTransfer));
    QMetaObject::invokeMethod(m_publicImpl, "transfersChanged", Qt::QueuedConnection);
    QMetaObject::invokeMethod(m_publicImpl, "transferAdded", Qt::QueuedConnection, Q_ARG(QFileTransfer, newTransfer));
}

void QRemoteFileSystemControllerImpl::transferProgressChanged(std::shared_ptr<FileTransfer> transfer)
{
    auto it = std::find_if(m_transfers.begin(), m_transfers.end(), [&transfer](const QVariant& el) {
        return qvariant_cast<QFileTransfer>(el).data() == transfer;
    });
    assert(it != m_transfers.end());
    if (it != m_transfers.end())
    {
        QMetaObject::invokeMethod(m_publicImpl, "transfersChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "transferUpdated", Qt::QueuedConnection,
                                  Q_ARG(QFileTransfer, qvariant_cast<QFileTransfer>(*it)));
    }
}

void QRemoteFileSystemControllerImpl::transferCompleted(std::shared_ptr<FileTransfer> transfer)
{
    auto it = std::find_if(m_transfers.begin(), m_transfers.end(), [&transfer](const QVariant& el) {
        return qvariant_cast<QFileTransfer>(el).data() == transfer;
    });

    assert(it != m_transfers.end());
    if (it != m_transfers.end())
    {
        QMetaObject::invokeMethod(m_publicImpl, "transferRemoved", Qt::QueuedConnection,
                                  Q_ARG(QFileTransfer, qvariant_cast<QFileTransfer>(*it)));
        m_transfers.erase(it);
        QMetaObject::invokeMethod(m_publicImpl, "transfersChanged", Qt::QueuedConnection);
    }
}

void QRemoteFileSystemControllerImpl::transferAborted(std::shared_ptr<FileTransfer> transfer)
{
    auto it = std::find_if(m_transfers.begin(), m_transfers.end(), [&transfer](const QVariant& el) {
        return qvariant_cast<QFileTransfer>(el).data() == transfer;
    });
    assert(it != m_transfers.end());
    if (it != m_transfers.end())
    {
        QMetaObject::invokeMethod(m_publicImpl, "transferRemoved", Qt::QueuedConnection,
                                  Q_ARG(QFileTransfer, qvariant_cast<QFileTransfer>(*it)));
        m_transfers.erase(it);
        QMetaObject::invokeMethod(m_publicImpl, "transfersChanged", Qt::QueuedConnection);
    }
}

void QRemoteFileSystemControllerImpl::errorOccured(const std::string& errorString)
{
    QMetaObject::invokeMethod(m_publicImpl, "error", Qt::QueuedConnection,
                              Q_ARG(QString, QString::fromStdString(errorString)));
}
