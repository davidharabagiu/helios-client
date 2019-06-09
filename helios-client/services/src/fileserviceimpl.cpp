#include <sstream>
#include <tuple>
#include <QDebug>
#include <fstream>
#include <utility>
#include <optional>
#include <cstdint>
#include <cassert>
#include <condition_variable>
#include <cstring>

#include "fileserviceimpl.h"
#include "apicalldefs.h"
#include "typeconversions.h"
#include "config.h"
#include "configkeys.h"
#include "autoresetevent.h"
#include "manualresetevent.h"
#include "pathutils.h"
#include "keymanager.h"
#include "fileapicaller.h"
#include "notificationsapicaller.h"

const size_t FileServiceImpl::s_kAcceptableKeyLength = 32;

FileServiceImpl::FileServiceImpl(std::shared_ptr<Config> config, std::unique_ptr<FileApiCaller> fileApiCaller,
                                 std::unique_ptr<NotificationsApiCaller> notificationsApiCaller,
                                 std::unique_ptr<CipherFactory> cipherFactory, std::shared_ptr<KeyManager> keyManager)
    : m_apiCaller(std::move(fileApiCaller))
    , m_notificationsApiCaller(std::move(notificationsApiCaller))
    , m_config(config)
    , m_lastUsedExecutorIndex(0)
    , m_numberOfTransferExecutors(m_config->get(ConfigKeys::kNumberOfTransferExecutors).toUInt())
    , m_cipherFactory(std::move(cipherFactory))
    , m_numberOfCipherExecutors(m_config->get(ConfigKeys::kNumberOfCipherExecutors).toUInt())
    , m_keyManager(keyManager)
{
    for (unsigned int i = 0; i < m_numberOfTransferExecutors; ++i)
    {
        m_transferExecutors.push_back(std::make_unique<Executor>());
    }
}

FileServiceImpl::~FileServiceImpl()
{
    for (auto& transfer : m_activeTransfers)
    {
        transfer.second->canceled = true;
    }

    // Wait for executors to complete
    std::unique_lock<std::mutex> lock(m_mutex);
    m_transfersCompletedCondVar.wait(lock, [this] { return m_activeTransfers.empty(); });
}

void FileServiceImpl::setSession(const UserSession& session)
{
    removeSession();

    m_apiCaller->list(
        session.authToken(), "",
        [this, session](ApiCallStatus                                                              status,
                        const std::vector<std::tuple<std::string, bool, std::optional<uint64_t>>>& files) {
            if (status == ApiCallStatus::SUCCESS)
            {
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    AuthenticatedService::setSession(session);
                    collectApiFileList(files);
                }
                Observable::notifyAll(&FileServiceListener::currentDirectoryChanged);
            }
            else if (status == ApiCallStatus::UNAUTHORIZED)
            {
                Observable::notifyAll(&FileServiceListener::errorOccured, "Authentication token invalid");
            }
            else
            {
                std::ostringstream ss;
                ss << "Error while listing files in the root directory. ApiCallStatus = ";
                ss << static_cast<int>(status);
                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            }
        });
}

void FileServiceImpl::removeSession()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        AuthenticatedService::removeSession();
        m_currentDirectory.clear();
        m_files.clear();
        m_activeTransfers.clear();
    }
    Observable::cancelPendingNotifications();
}

std::string FileServiceImpl::currentDirectory() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_currentDirectory;
}

std::vector<std::shared_ptr<const File>> FileServiceImpl::files() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    std::vector<std::shared_ptr<const File>> filesVec;
    filesVec.reserve(m_files.size());

    for (const auto& file : m_files)
    {
        filesVec.push_back(file.second);
    }

    return filesVec;
}

std::vector<std::shared_ptr<FileTransfer>> FileServiceImpl::activeTransfers() const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    std::vector<std::shared_ptr<FileTransfer>> fileTransfersVec;
    fileTransfersVec.reserve(m_activeTransfers.size());

    for (const auto& transfer : m_activeTransfers)
    {
        fileTransfersVec.push_back(std::make_shared<FileTransfer>(*(transfer.second->transfer)));
    }

    return fileTransfersVec;
}

void FileServiceImpl::changeCurrentDirectory(const std::string& path, bool relative)
{
    if (!enabled())
    {
        return;
    }

    std::string fullPath;
    if (relative)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        fullPath = PathUtils::concatenatePaths(m_currentDirectory, path);
    }
    else
    {
        fullPath = path;
        PathUtils::removeTrailingSlash(fullPath);
    }

    m_apiCaller->list(
        m_session.authToken(), fullPath,
        [this, fullPath](ApiCallStatus                                                              status,
                         const std::vector<std::tuple<std::string, bool, std::optional<uint64_t>>>& files) {
            if (status == ApiCallStatus::SUCCESS)
            {
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_currentDirectory = fullPath;
                    collectApiFileList(files);
                }
                Observable::notifyAll(&FileServiceListener::currentDirectoryChanged);
            }
            else if (status == ApiCallStatus::INVALID_PATH)
            {
                std::ostringstream ss;
                ss << "Path ";
                ss << fullPath;
                ss << " is invalid.";
                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            }
            else
            {
                std::ostringstream ss;
                ss << "Error while listing files. ApiCallStatus = ";
                ss << static_cast<int>(status);
                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            }
        });
}

void FileServiceImpl::navigateBack()
{
    if (m_currentDirectory == "")
    {
        return;
    }

    std::string dir;
    std::string parent;
    PathUtils::getFileNameAndParentDir(m_currentDirectory, dir, parent);
    changeCurrentDirectory(parent, false);
}

void FileServiceImpl::createDirectory(const std::string& path, bool relative)
{
    if (!enabled())
    {
        return;
    }

    std::string fullPath;
    if (relative)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        fullPath = PathUtils::concatenatePaths(m_currentDirectory, path);
    }
    else
    {
        fullPath = path;
        PathUtils::removeTrailingSlash(fullPath);
    }

    m_apiCaller->createDirectory(m_session.authToken(), fullPath, [this, fullPath](ApiCallStatus status) {
        if (status == ApiCallStatus::SUCCESS)
        {
            std::string dirName;
            std::string dirParent;
            PathUtils::getFileNameAndParentDir(fullPath, dirName, dirParent);

            auto dir = std::make_shared<File>(dirName, dirParent, true);

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (dirParent == m_currentDirectory)
                {
                    m_files.emplace(dirName, dir);
                }
            }

            Observable::notifyAll(&FileServiceListener::directoryCreated, dir);
        }
        else if (status == ApiCallStatus::INVALID_PATH)
        {
            std::ostringstream ss;
            ss << "Path ";
            ss << fullPath;
            ss << " is invalid.";
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
        else if (status == ApiCallStatus::FILE_ALREADY_EXISTS)
        {
            std::ostringstream ss;
            ss << "Directory ";
            ss << fullPath;
            ss << " already exists.";
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
        else
        {
            std::ostringstream ss;
            ss << "Error while creating directory. ApiCallStatus = ";
            ss << static_cast<int>(status);
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
    });
}

void FileServiceImpl::uploadFile(const std::string& localPath, const std::string& remotePath, bool relative,
                                 const std::string& encryptionKeyName)
{
    auto encryptionKey = m_keyManager->getKey(encryptionKeyName);
    if (encryptionKey.size() != s_kAcceptableKeyLength)
    {
        std::ostringstream ss;
        ss << "There is no key of valid length with the name ";
        ss << encryptionKeyName;
        Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        return;
    }

    // Obtain the full remote path
    std::string fullRemotePath;
    if (relative)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        fullRemotePath = PathUtils::concatenatePaths(m_currentDirectory, remotePath);
    }
    else
    {
        fullRemotePath = remotePath;
        PathUtils::removeTrailingSlash(fullRemotePath);
    }

    // Check if there is already an active transfer on this file
    {
        bool shouldFail = false;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            shouldFail = m_activeTransfers.find(fullRemotePath) != m_activeTransfers.end();
        }
        if (shouldFail)
        {
            std::ostringstream ss;
            ss << "There is already an active transfer on file ";
            ss << fullRemotePath;
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            return;
        }
    }

    auto cipher = m_cipherFactory->createCipher(CipherFactory::Algorithm::AES256,
                                                safe_integral_cast<int>(m_numberOfCipherExecutors));
    cipher->setKey(encryptionKey.data());

    m_apiCaller->beginUpload(
        m_session.authToken(), fullRemotePath,
        [this, localPath, fullRemotePath, cipher](ApiCallStatus status, const std::string& transferId) mutable {
            if (status == ApiCallStatus::SUCCESS)
            {
                // Obtain the file size
                uint64_t fileSize;
                {
                    std::ifstream stream(localPath, std::ifstream::ate | std::ifstream::binary);
                    if (!stream.good())
                    {
                        std::ostringstream ss;
                        ss << "Error while reading ";
                        ss << localPath;
                        Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
                    }
                    fileSize = safe_integral_cast<uint64_t>(static_cast<long long>(stream.tellg()));
                }

                auto transfer      = std::make_shared<FileTransferInternal>();
                transfer->transfer = std::make_shared<FileTransfer>(localPath, fullRemotePath,
                                                                    FileTransfer::TransferMode::UPLOAD, fileSize);
                transfer->stream   = std::make_shared<std::ifstream>(localPath, std::ios::binary);
                m_activeTransfers.emplace(fullRemotePath, transfer);

                Observable::notifyAll(&FileServiceListener::transferStarted, transfer->transfer);

                // Post transfer execution
                m_transferExecutors[nextExecutorIndex()]->post([this, transfer, transferId, fullRemotePath,
                                                                cipher]() mutable {
                    uint64_t chunkSize =
                        safe_integral_cast<uint64_t>(m_config->get(ConfigKeys::kUploadChunkSize).toUInt()) * 1024;
                    auto                       stream = std::static_pointer_cast<std::ifstream>(transfer->stream);
                    std::unique_ptr<uint8_t[]> buffer(new uint8_t[chunkSize + sizeof(fileSize)]);

                    auto     fileSize    = transfer->transfer->fileSize();
                    uint64_t transferred = transfer->transfer->transferredBytes();

                    AutoResetEvent chunkDone;

                    // Prepare first transfer
                    uint64_t read = (transferred + chunkSize > fileSize) ? fileSize - transferred : chunkSize;
                    std::memcpy(buffer.get(), &fileSize, sizeof(fileSize));
                    stream->read(reinterpret_cast<char*>(buffer.get() + sizeof(fileSize)),
                                 safe_integral_cast<std::streamsize>(read));
                    uint64_t encryptedBytes =
                        cipher->encrypt(buffer.get() + sizeof(fileSize), read, buffer.get() + sizeof(fileSize));
                    uint64_t totalBytesToSend = encryptedBytes + sizeof(fileSize);
                    uint64_t totalBytesSent   = 0;

                    while (transferred < fileSize && !transfer->canceled)
                    {
                        ApiCallStatus lastStatus;
                        m_apiCaller->upload(m_session.authToken(), transferId, totalBytesSent,
                                            std::vector<uint8_t>(buffer.get(), buffer.get() + totalBytesToSend),
                                            [&chunkDone, &lastStatus](ApiCallStatus status) {
                                                lastStatus = status;
                                                chunkDone.set();
                                            });
                        totalBytesSent += totalBytesToSend;

                        // Save current value of "encryptedBytes"
                        uint64_t _encryptedBytes = encryptedBytes;

                        // Prepare next transfer if necessary
                        if (transferred + read < fileSize)
                        {
                            read =
                                (transferred + read + chunkSize > fileSize) ? fileSize - transferred - read : chunkSize;
                            stream->read(reinterpret_cast<char*>(buffer.get()),
                                         safe_integral_cast<std::streamsize>(read));
                            totalBytesToSend = encryptedBytes = cipher->encrypt(buffer.get(), read, buffer.get());
                        }

                        // Wait for current transfer to finish
                        chunkDone.wait();

                        if (lastStatus == ApiCallStatus::SUCCESS)
                        {
                            transferred += _encryptedBytes;
                            transfer->transfer->setTransferredBytes(transferred);
                            Observable::notifyAll(&FileServiceListener::transferProgressChanged, transfer->transfer);
                        }
                        else
                        {
                            // Upload failure
                            Observable::notifyAll(&FileServiceListener::transferAborted, transfer->transfer);
                            std::ostringstream ss;
                            ss << "Error while uploading. ApiCallStatus = ";
                            ss << static_cast<int>(lastStatus);
                            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
                            break;
                        }
                    }

                    // Cleanup
                    {
                        std::lock_guard<std::mutex> lock(m_mutex);
                        m_activeTransfers.erase(fullRemotePath);
                    }

                    if (transferred >= fileSize)
                    {
                        // Notify completion
                        Observable::notifyAll(&FileServiceListener::transferCompleted, transfer->transfer);
                    }
                    else
                    {
                        Observable::notifyAll(&FileServiceListener::transferAborted, transfer->transfer);
                    }

                    {
                        std::string remoteFileName;
                        std::string remoteFileParent;
                        PathUtils::getFileNameAndParentDir(fullRemotePath, remoteFileName, remoteFileParent);
                        std::lock_guard<std::mutex> lock(m_mutex);
                        if (remoteFileParent == m_currentDirectory)
                        {
                            auto file = std::make_shared<File>(remoteFileName, remoteFileParent, false, totalBytesSent);
                            m_files[remoteFileName] = file;
                            Observable::notifyAll(&FileServiceListener::uploadedFileInCurrentDir, file);
                        }
                    }

                    cipher.reset();

                    m_transfersCompletedCondVar.notify_one();
                });
            }
            else if (status == ApiCallStatus::INVALID_PATH)
            {
                std::ostringstream ss;
                ss << "Path ";
                ss << fullRemotePath;
                ss << " is invalid.";
                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            }
            else
            {
                std::ostringstream ss;
                ss << "Error while starting upload. ApiCallStatus = ";
                ss << static_cast<int>(status);
                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            }
        });
}

void FileServiceImpl::downloadFile(const std::string& remotePath, bool relative, const std::string& localPath,
                                   const std::string& decryptionKeyName)
{
    auto decryptionKey = m_keyManager->getKey(decryptionKeyName);
    if (decryptionKey.size() != s_kAcceptableKeyLength)
    {
        std::ostringstream ss;
        ss << "There is no key with the name ";
        ss << decryptionKeyName;
        Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        return;
    }

    // Obtain the full remote path
    std::string fullRemotePath;
    if (relative)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        fullRemotePath = PathUtils::concatenatePaths(m_currentDirectory, remotePath);
    }
    else
    {
        fullRemotePath = remotePath;
        PathUtils::removeTrailingSlash(fullRemotePath);
    }

    // Check if there is already an active transfer on this file
    {
        bool shouldFail = false;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            shouldFail = m_activeTransfers.find(fullRemotePath) != m_activeTransfers.end();
        }
        if (shouldFail)
        {
            std::ostringstream ss;
            ss << "There is already an active transfer on file ";
            ss << fullRemotePath;
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            return;
        }
    }

    auto cipher = m_cipherFactory->createCipher(CipherFactory::Algorithm::AES256,
                                                safe_integral_cast<int>(m_numberOfCipherExecutors));
    cipher->setKey(decryptionKey.data());

    m_apiCaller->beginDownload(
        m_session.authToken(), fullRemotePath,
        [this, localPath, fullRemotePath, cipher](ApiCallStatus status, const std::string& transferId,
                                                  uint64_t fileSize) mutable {
            if (status == ApiCallStatus::SUCCESS)
            {
                auto transfer      = std::make_shared<FileTransferInternal>();
                transfer->transfer = std::make_shared<FileTransfer>(localPath, fullRemotePath,
                                                                    FileTransfer::TransferMode::DOWNLOAD, fileSize);
                transfer->stream   = std::make_shared<std::ofstream>(localPath, std::ios::binary);
                m_activeTransfers.emplace(fullRemotePath, transfer);

                Observable::notifyAll(&FileServiceListener::transferStarted, transfer->transfer);

                // Post transfer execution
                m_transferExecutors[nextExecutorIndex()]->post([this, transfer, transferId, fullRemotePath,
                                                                cipher]() mutable {
                    uint64_t chunkSize =
                        safe_integral_cast<uint64_t>(m_config->get(ConfigKeys::kUploadChunkSize).toUInt()) * 1024;
                    auto                       stream = std::static_pointer_cast<std::ofstream>(transfer->stream);
                    std::unique_ptr<uint8_t[]> buffer(new uint8_t[chunkSize + sizeof(uint64_t)]);

                    auto     fileSize    = transfer->transfer->fileSize();
                    uint64_t transferred = transfer->transfer->transferredBytes();
                    uint64_t processed   = 0;
                    uint64_t written     = 0;

                    AutoResetEvent   chunkDone;
                    ManualResetEvent decryptionDone;

                    ApiCallStatus lastStatus;
                    uint64_t      lastTransferred;

                    // Actual file size before encryption
                    uint64_t actualFileSize;
                    bool     firstTransfer   = true;
                    bool     firstProcessing = true;

                    while (processed < fileSize && !transfer->canceled)
                    {
                        uint64_t _offset         = firstTransfer ? sizeof(actualFileSize) : 0;
                        uint64_t actualChunkSize = (transferred + chunkSize + _offset > fileSize) ?
                                                       fileSize - transferred :
                                                       chunkSize + _offset;
                        firstTransfer = false;

                        if (transferred < fileSize)
                        {
                            // We have data left to download
                            m_apiCaller->download(
                                m_session.authToken(), transferId, transferred, actualChunkSize,
                                [&chunkDone, &lastStatus, &transferred, &buffer, &lastTransferred, &decryptionDone](
                                    ApiCallStatus status, const std::vector<uint8_t>& bytes) {
                                    lastStatus = status;
                                    if (status == ApiCallStatus::SUCCESS)
                                    {
                                        decryptionDone.wait();
                                        std::copy(bytes.cbegin(), bytes.cend(), buffer.get());
                                        transferred += bytes.size();
                                        lastTransferred = safe_integral_cast<uint64_t>(bytes.size());
                                    }
                                    chunkDone.set();
                                });
                        }

                        decryptionDone.reset();
                        if (transferred > 0)
                        {
                            if (firstProcessing)
                            {
                                std::memcpy(&actualFileSize, buffer.get(), sizeof(actualFileSize));
                            }

                            // We have data for decryption
                            auto     _offset       = firstProcessing ? sizeof(actualFileSize) : 0;
                            uint64_t paddingLength = 0;
                            cipher->decrypt(buffer.get() + _offset, lastTransferred - _offset, buffer.get() + _offset);

                            // Remove padding
                            if (written + lastTransferred - _offset > actualFileSize)
                            {
                                paddingLength = written + lastTransferred - _offset - actualFileSize;
                            }

                            stream->write(
                                reinterpret_cast<const char*>(buffer.get() + _offset),
                                safe_integral_cast<std::streamsize>(lastTransferred - _offset - paddingLength));
                            written += lastTransferred - _offset - paddingLength;

                            if (lastStatus == ApiCallStatus::SUCCESS)
                            {
                                transfer->transfer->setTransferredBytes(transferred);
                                Observable::notifyAll(&FileServiceListener::transferProgressChanged,
                                                      transfer->transfer);
                            }
                            else
                            {
                                // Upload failure
                                std::ostringstream ss;
                                ss << "Error while downloading. ApiCallStatus = ";
                                ss << static_cast<int>(lastStatus);
                                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
                                break;
                            }

                            processed += lastTransferred;
                            firstProcessing = false;
                        }
                        decryptionDone.set();

                        if (transferred < fileSize)
                        {
                            chunkDone.wait();
                        }
                    }

                    // Cleanup
                    {
                        std::lock_guard<std::mutex> lock(m_mutex);
                        m_activeTransfers.erase(fullRemotePath);
                    }

                    if (processed == fileSize)
                    {
                        // Notify completion
                        Observable::notifyAll(&FileServiceListener::transferCompleted, transfer->transfer);
                    }
                    else
                    {
                        Observable::notifyAll(&FileServiceListener::transferAborted, transfer->transfer);
                    }

                    cipher.reset();

                    m_transfersCompletedCondVar.notify_one();
                });
            }
            else if (status == ApiCallStatus::INVALID_PATH)
            {
                std::ostringstream ss;
                ss << "Path ";
                ss << fullRemotePath;
                ss << " is invalid.";
                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            }
            else
            {
                std::ostringstream ss;
                ss << "Error while starting download. ApiCallStatus = ";
                ss << static_cast<int>(status);
                Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
            }
        });
}

void FileServiceImpl::cancelOperation(const std::string& path)
{
    auto it = m_activeTransfers.find(path);
    if (it != m_activeTransfers.end())
    {
        it->second->canceled = true;
    }
}

void FileServiceImpl::moveFile(const std::string& sourcePath, const std::string& destinationPath)
{
    if (!enabled())
    {
        return;
    }

    std::string source(sourcePath);
    PathUtils::removeTrailingSlash(source);

    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_activeTransfers.find(source) != m_activeTransfers.end())
        {
            lock.release();
            std::ostringstream ss;
            ss << "File ";
            ss << source;
            ss << " is in use.";
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
    }

    std::string destination(destinationPath);
    PathUtils::removeTrailingSlash(destination);

    m_apiCaller->move(m_session.authToken(), source, destination, [this, source, destination](ApiCallStatus status) {
        if (status == ApiCallStatus::SUCCESS)
        {
            m_apiCaller->isDir(
                m_session.authToken(), destination, [this, source, destination](ApiCallStatus status, bool isDir) {
                    if (status == ApiCallStatus::SUCCESS)
                    {
                        if (isDir)
                        {
                            completeMove(source, destination, isDir);
                        }
                        else
                        {
                            m_apiCaller->getFileSize(m_session.authToken(), destination,
                                                     [this, source, destination](ApiCallStatus status, uint64_t size) {
                                                         if (status == ApiCallStatus::SUCCESS)
                                                         {
                                                             completeMove(source, destination, false, size);
                                                         }
                                                         else
                                                         {
                                                             std::ostringstream ss;
                                                             ss << "Error while querying file size. ApiCallStatus = ";
                                                             ss << static_cast<int>(status);
                                                             Observable::notifyAll(&FileServiceListener::errorOccured,
                                                                                   ss.str());
                                                         }
                                                     });
                        }
                    }
                    else
                    {
                        std::ostringstream ss;
                        ss << "Error while checking if a file is a directory. ApiCallStatus = ";
                        ss << static_cast<int>(status);
                        Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
                    }
                });
        }
        else if (status == ApiCallStatus::INVALID_PATH)
        {
            std::ostringstream ss;
            ss << "Path ";
            ss << source;
            ss << " is invalid.";
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
        else if (status == ApiCallStatus::FILE_ALREADY_EXISTS)
        {
            std::ostringstream ss;
            ss << "File ";
            ss << destination;
            ss << " already exists.";
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
        else
        {
            std::ostringstream ss;
            ss << "Error while moving file. ApiCallStatus = ";
            ss << static_cast<int>(status);
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
    });
}

void FileServiceImpl::removeFile(const std::string& path, bool relative)
{
    if (!enabled())
    {
        return;
    }

    std::string fullPath;
    if (relative)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        fullPath = PathUtils::concatenatePaths(m_currentDirectory, path);
    }
    else
    {
        fullPath = path;
        PathUtils::removeTrailingSlash(fullPath);
    }

    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_activeTransfers.find(fullPath) != m_activeTransfers.end())
        {
            lock.release();
            std::ostringstream ss;
            ss << "File ";
            ss << fullPath;
            ss << " is in use.";
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
    }

    m_apiCaller->remove(m_session.authToken(), fullPath, [this, fullPath](ApiCallStatus status) {
        if (status == ApiCallStatus::SUCCESS)
        {
            std::string name;
            std::string parent;
            PathUtils::getFileNameAndParentDir(fullPath, name, parent);

            auto file = std::make_shared<File>(name, parent, false);

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (parent == m_currentDirectory)
                {
                    m_files.erase(name);
                }
            }

            Observable::notifyAll(&FileServiceListener::fileRemoved, file);
        }
        else if (status == ApiCallStatus::INVALID_PATH)
        {
            std::ostringstream ss;
            ss << "Path ";
            ss << fullPath;
            ss << " is invalid.";
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
        else
        {
            std::ostringstream ss;
            ss << "Error while removing file. ApiCallStatus = ";
            ss << static_cast<int>(status);
            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
        }
    });
}

void FileServiceImpl::shareFile(const std::string& user, const std::string& path, bool relative)
{
    if (!enabled())
    {
        return;
    }

    std::string fullPath;
    if (relative)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        fullPath = PathUtils::concatenatePaths(m_currentDirectory, path);
    }
    else
    {
        fullPath = path;
        PathUtils::removeTrailingSlash(fullPath);
    }

    m_apiCaller->share(m_session.authToken(), user, fullPath, [this, user, fullPath](ApiCallStatus status) {
        if (status == ApiCallStatus::SUCCESS)
        {
            Observable::notifyAll(&FileServiceListener::fileShared);
        }
        else if (status == ApiCallStatus::INVALID_USERNAME)
        {
            Observable::notifyAll(&FileServiceListener::errorOccured, "User " + user + " not found");
        }
        else if (status == ApiCallStatus::INVALID_PATH)
        {
            Observable::notifyAll(&FileServiceListener::errorOccured, "Path " + fullPath + " is invalid");
        }
        else
        {
            Observable::notifyAll(&FileServiceListener::errorOccured, "Unknown error occured while sharing file");
        }
    });
}

void FileServiceImpl::acceptSharedFile(const std::string& notificationId, const std::string& path, bool relative)
{
    if (!enabled())
    {
        return;
    }

    std::string fullPath;
    if (relative)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        fullPath = PathUtils::concatenatePaths(m_currentDirectory, path);
    }
    else
    {
        fullPath = path;
        PathUtils::removeTrailingSlash(fullPath);
    }

    m_apiCaller->acceptFile(
        m_session.authToken(), notificationId, fullPath, [this, fullPath, notificationId](ApiCallStatus status) {
            if (status == ApiCallStatus::SUCCESS)
            {
                m_apiCaller->getFileSize(
                    m_session.authToken(), fullPath,
                    [this, fullPath, notificationId](ApiCallStatus status, uint64_t fileSize) {
                        if (status == ApiCallStatus::SUCCESS)
                        {
                            m_notificationsApiCaller->dismissNotification(
                                m_session.authToken(), notificationId,
                                [this, fullPath, fileSize](ApiCallStatus status) {
                                    if (status == ApiCallStatus::SUCCESS)
                                    {
                                        std::string name;
                                        std::string parent;
                                        PathUtils::getFileNameAndParentDir(fullPath, name, parent);

                                        auto newFile = std::make_shared<File>(name, parent, false, fileSize);

                                        {
                                            std::lock_guard<std::mutex> lock(m_mutex);
                                            if (parent == m_currentDirectory)
                                            {
                                                m_files.emplace(name, newFile);
                                            }
                                        }

                                        Observable::notifyAll(&FileServiceListener::acceptedFileShare, newFile);
                                    }
                                    else
                                    {
                                        std::ostringstream error;
                                        error << "Unhandled error while dismissing notification: "
                                              << static_cast<int>(status);
                                        Observable::notifyAll(&FileServiceListener::errorOccured, error.str());
                                    }
                                });
                        }
                        else
                        {
                            std::ostringstream error;
                            error << "Unhandled error while querying file size for " << fullPath << ": "
                                  << static_cast<int>(status);
                            Observable::notifyAll(&FileServiceListener::errorOccured, error.str());
                        }
                    });
            }
            else if (status == ApiCallStatus::FILE_NO_LONGER_EXISTS)
            {
                Observable::notifyAll(&FileServiceListener::errorOccured, "The requested file no longer exists");
            }
            else if (status == ApiCallStatus::FILE_ALREADY_EXISTS)
            {
                std::ostringstream error;
                error << fullPath << " already exists";
                Observable::notifyAll(&FileServiceListener::errorOccured, error.str());
            }
            else if (status == ApiCallStatus::INVALID_PATH)
            {
                std::ostringstream error;
                error << "Path" << fullPath << " already exists";
                Observable::notifyAll(&FileServiceListener::errorOccured, error.str());
            }
            else
            {
                std::ostringstream error;
                error << "Unhandled error while accepting the file: " << static_cast<int>(status);
                Observable::notifyAll(&FileServiceListener::errorOccured, error.str());
            }
        });
}

void FileServiceImpl::collectApiFileList(
    const std::vector<std::tuple<std::string, bool, std::optional<uint64_t>>>& files)
{
    m_files.clear();
    for (const auto& apiFile : files)
    {
        auto name  = std::get<0>(apiFile);
        bool isDir = std::get<1>(apiFile);
        auto size  = std::get<2>(apiFile);
        assert(isDir || (!isDir && size.has_value()));
        assert(!isDir || (isDir && !size.has_value()));

        m_files.emplace(name,
                        std::make_shared<File>(std::get<0>(apiFile), m_currentDirectory, isDir, isDir ? 0 : *size));
    }
}

void FileServiceImpl::completeMove(const std::string& source, const std::string& destination, bool isDir, uint64_t size)
{
    std::string sourceName;
    std::string sourceParent;
    PathUtils::getFileNameAndParentDir(source, sourceName, sourceParent);

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (sourceParent == m_currentDirectory)
        {
            m_files.erase(sourceName);
        }
    }

    std::string destinationName;
    std::string destinationParent;
    PathUtils::getFileNameAndParentDir(destination, destinationName, destinationParent);

    auto oldFile = std::make_shared<File>(sourceName, sourceParent, isDir, size);
    auto file    = std::make_shared<File>(destinationName, destinationParent, isDir, size);

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (destinationParent == m_currentDirectory)
        {
            m_files.emplace(destinationName, file);
        }
    }

    Observable::notifyAll(&FileServiceListener::fileMoved, oldFile, file);
}

unsigned FileServiceImpl::nextExecutorIndex()
{
    unsigned index          = m_lastUsedExecutorIndex;
    m_lastUsedExecutorIndex = (m_lastUsedExecutorIndex + 1) % m_numberOfTransferExecutors;
    return index;
}
