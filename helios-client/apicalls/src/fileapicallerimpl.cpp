#include <QDebug>

#include "fileapicallerimpl.h"
#include "single.h"
#include "dependencyinjector.h"
#include "httprequestmanager.h"
#include "apicallvisitor.h"
#include "createdirectorycall.h"
#include "beginuploadcall.h"
#include "uploadcall.h"
#include "begindownloadcall.h"
#include "downloadcall.h"
#include "endtransfercall.h"
#include "getfilesizecall.h"
#include "listcall.h"
#include "removecall.h"
#include "movecall.h"
#include "isdircall.h"
#include "sharekeycall.h"
#include "acceptkeycall.h"
#include "sharecall.h"
#include "acceptfilecall.h"

FileApiCallerImpl::FileApiCallerImpl(const std::shared_ptr<HttpRequestManager>& requestManager)
{
    m_visitor = std::make_shared<ApiCallVisitor>(requestManager);
}

void FileApiCallerImpl::createDirectory(const std::string& authToken, const std::string& path,
                                        const ApiCallbacks::CreateDirectoryCallback& callback) const
{
    auto call = new CreateDirectoryCall(authToken, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::beginUpload(const std::string& authToken, const std::string& path,
                                    const ApiCallbacks::BeginUploadCallback& callback) const
{
    auto call = new BeginUploadCall(authToken, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::upload(const std::string& authToken, const std::string& transferId, uint64_t offset,
                               const std::vector<uint8_t>& content, const ApiCallbacks::UploadCallback& callback) const
{
    auto call = new UploadCall(authToken, transferId, offset, content, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::beginDownload(const std::string& authToken, const std::string& path,
                                      const ApiCallbacks::BeginDownloadCallback& callback) const
{
    auto call = new BeginDownloadCall(authToken, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::download(const std::string& authToken, const std::string& transferId, uint64_t offset,
                                 uint64_t size, const ApiCallbacks::DownloadCallback& callback) const
{
    auto call = new DownloadCall(authToken, transferId, offset, size, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::endTransfer(const std::string& authToken, const std::string& transferId,
                                    const ApiCallbacks::EndTransferCallback& callback) const
{
    auto call = new EndTransferCall(authToken, transferId, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::getFileSize(const std::string& authToken, const std::string& path,
                                    const ApiCallbacks::GetFileSizeCallback& callback) const
{
    auto call = new GetFileSizeCall(authToken, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::list(const std::string& authToken, const std::string& path,
                             const ApiCallbacks::ListCallback& callback) const
{
    auto call = new ListCall(authToken, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::remove(const std::string& authToken, const std::string& path,
                               const ApiCallbacks::RemoveCallback& callback) const
{
    auto call = new RemoveCall(authToken, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::move(const std::string& authToken, const std::string& source, const std::string& destination,
                             const ApiCallbacks::MoveCallback& callback) const
{
    auto call = new MoveCall(authToken, source, destination, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::isDir(const std::string& authToken, const std::string& path,
                              const ApiCallbacks::IsDirCallback& callback) const
{
    auto call = new IsDirCall(authToken, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::shareKey(const std::string& authToken, const std::string& username, const std::string& keyName,
                                 uint16_t keyLength, const std::vector<uint8_t>& keyContent,
                                 const ApiCallbacks::ShareKeyCallback& callback) const
{
    auto call = new ShareKeyCall(authToken, username, keyName, keyLength, keyContent, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::acceptKey(const std::string& authToken, const std::string& notificationId,
                                  const ApiCallbacks::AcceptKeyCallback& callback) const
{
    auto call = new AcceptKeyCall(authToken, notificationId, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::share(const std::string& authToken, const std::string& username, const std::string& path,
                              const ApiCallbacks::ShareCallback& callback) const
{
    auto call = new ShareCall(authToken, username, path, callback);
    call->send(m_visitor);
}

void FileApiCallerImpl::acceptFile(const std::string& authToken, const std::string& notificationId,
                                   const std::string& path, const ApiCallbacks::AcceptFileCallback& callback) const
{
    auto call = new AcceptFileCall(authToken, notificationId, path, callback);
    call->send(m_visitor);
}
