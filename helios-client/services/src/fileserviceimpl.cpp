#include <sstream>
#include <tuple>
#include <QDebug>
#include <fstream>
#include <utility>
#include <optional>
#include <cstdint>
#include <cassert>

#include "fileserviceimpl.h"
#include "apicalldefs.h"
#include "typeconversions.h"
#include "config.h"
#include "configkeys.h"
#include "autoresetevent.h"
#include "pathutils.h"

FileServiceImpl::FileServiceImpl(std::shared_ptr<Config> config, std::unique_ptr<FileApiCaller> fileApiCaller)
    : m_apiCaller(std::move(fileApiCaller))
    , m_config(config)
{
}

bool FileServiceImpl::enabled() const
{
    return !m_authToken.empty();
}

void FileServiceImpl::setAuthToken(const std::string& authToken)
{
    removeAuthToken();

    m_apiCaller->list(
        authToken, "",
        [this, authToken](ApiCallStatus                                                              status,
                          const std::vector<std::tuple<std::string, bool, std::optional<uint64_t>>>& files) {
            if (status == ApiCallStatus::SUCCESS)
            {
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_authToken = authToken;
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

void FileServiceImpl::removeAuthToken()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_authToken.clear();
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
        m_authToken, fullPath,
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

    m_apiCaller->createDirectory(m_authToken, fullPath, [this, fullPath](ApiCallStatus status) {
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

void FileServiceImpl::uploadFile(const std::string& localPath, const std::string& remotePath, bool relative)
{
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

    m_apiCaller->beginUpload(
        m_authToken, fullRemotePath,
        [this, localPath, fullRemotePath](ApiCallStatus status, const std::string& transferId) {
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

                // Post transfer execution
                m_transfersExecutor.post([this, transfer, transferId, fullRemotePath] {
                    Observable::notifyAll(&FileServiceListener::transferStarted, transfer->transfer);

                    uint64_t chunkSize =
                        safe_integral_cast<uint64_t>(m_config->get(ConfigKeys::kUploadChunkSize).toUInt()) * 1024;
                    auto                       stream = std::static_pointer_cast<std::ifstream>(transfer->stream);
                    std::unique_ptr<uint8_t[]> buffer(new uint8_t[chunkSize]);

                    auto     fileSize    = transfer->transfer->fileSize();
                    uint64_t transferred = transfer->transfer->transferredBytes();

                    AutoResetEvent chunkDone;

                    while (transferred < fileSize)
                    {
                        uint64_t read = (transferred + chunkSize > fileSize) ? fileSize - transferred : chunkSize;
                        stream->read(reinterpret_cast<char*>(buffer.get()), safe_integral_cast<std::streamsize>(read));

                        ApiCallStatus lastStatus;
                        m_apiCaller->upload(m_authToken, transferId, transferred,
                                            std::vector<uint8_t>(buffer.get(), buffer.get() + read),
                                            [&chunkDone, &lastStatus](ApiCallStatus status) {
                                                lastStatus = status;
                                                chunkDone.set();
                                            });

                        chunkDone.wait();

                        if (lastStatus == ApiCallStatus::SUCCESS)
                        {
                            transferred += read;
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
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_activeTransfers.erase(fullRemotePath);

                    if (transferred == fileSize)
                    {
                        // Notify completion
                        Observable::notifyAll(&FileServiceListener::transferCompleted, transfer->transfer);
                    }
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

void FileServiceImpl::downloadFile(const std::string& remotePath, bool relative, const std::string& localPath)
{
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

    m_apiCaller->beginDownload(
        m_authToken, fullRemotePath,
        [this, localPath, fullRemotePath](ApiCallStatus status, const std::string& transferId, uint64_t fileSize) {
            if (status == ApiCallStatus::SUCCESS)
            {
                auto transfer      = std::make_shared<FileTransferInternal>();
                transfer->transfer = std::make_shared<FileTransfer>(localPath, fullRemotePath,
                                                                    FileTransfer::TransferMode::DOWNLOAD, fileSize);
                transfer->stream   = std::make_shared<std::ofstream>(localPath, std::ios::binary);
                m_activeTransfers.emplace(fullRemotePath, transfer);

                // Post transfer execution
                m_transfersExecutor.post([this, transfer, transferId, fullRemotePath] {
                    Observable::notifyAll(&FileServiceListener::transferStarted, transfer->transfer);

                    uint64_t chunkSize =
                        safe_integral_cast<uint64_t>(m_config->get(ConfigKeys::kUploadChunkSize).toUInt()) * 1024;
                    auto                       stream = std::static_pointer_cast<std::ofstream>(transfer->stream);
                    std::unique_ptr<uint8_t[]> buffer(new uint8_t[chunkSize]);

                    auto     fileSize    = transfer->transfer->fileSize();
                    uint64_t transferred = transfer->transfer->transferredBytes();

                    AutoResetEvent chunkDone;

                    while (transferred < fileSize)
                    {
                        ApiCallStatus lastStatus;
                        m_apiCaller->download(m_authToken, transferId, transferred, chunkSize,
                                              [&chunkDone, &lastStatus, &transferred, &stream](
                                                  ApiCallStatus status, const std::vector<uint8_t>& bytes) {
                                                  lastStatus = status;
                                                  if (status == ApiCallStatus::SUCCESS)
                                                  {
                                                      stream->write(reinterpret_cast<const char*>(bytes.data()),
                                                                    safe_integral_cast<long>(bytes.size()));
                                                      transferred += bytes.size();
                                                  }
                                                  chunkDone.set();
                                              });

                        chunkDone.wait();

                        if (lastStatus == ApiCallStatus::SUCCESS)
                        {
                            transfer->transfer->setTransferredBytes(transferred);
                            Observable::notifyAll(&FileServiceListener::transferProgressChanged, transfer->transfer);
                        }
                        else
                        {
                            // Upload failure
                            Observable::notifyAll(&FileServiceListener::transferAborted, transfer->transfer);
                            std::ostringstream ss;
                            ss << "Error while downloading. ApiCallStatus = ";
                            ss << static_cast<int>(lastStatus);
                            Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
                            break;
                        }
                    }

                    // Cleanup
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_activeTransfers.erase(fullRemotePath);

                    if (transferred == fileSize)
                    {
                        // Notify completion
                        Observable::notifyAll(&FileServiceListener::transferCompleted, transfer->transfer);
                    }
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

void FileServiceImpl::cancelOperation(const std::string& /*path*/)
{
    // TODO: Implement transfer cancelation
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

    m_apiCaller->move(m_authToken, source, destination, [this, source, destination](ApiCallStatus status) {
        if (status == ApiCallStatus::SUCCESS)
        {
            m_apiCaller->isDir(m_authToken, destination, [this, source, destination](ApiCallStatus status, bool isDir) {
                if (status == ApiCallStatus::SUCCESS)
                {
                    if (isDir)
                    {
                        completeMove(source, destination, isDir);
                    }
                    else
                    {
                        m_apiCaller->getFileSize(
                            m_authToken, destination, [this, source, destination](ApiCallStatus status, uint64_t size) {
                                if (status == ApiCallStatus::SUCCESS)
                                {
                                    completeMove(source, destination, false, size);
                                }
                                else
                                {
                                    std::ostringstream ss;
                                    ss << "Error while querying file size. ApiCallStatus = ";
                                    ss << static_cast<int>(status);
                                    Observable::notifyAll(&FileServiceListener::errorOccured, ss.str());
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

    m_apiCaller->remove(m_authToken, fullPath, [this, fullPath](ApiCallStatus status) {
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
