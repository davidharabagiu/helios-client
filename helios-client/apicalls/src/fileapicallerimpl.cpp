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

FileApiCallerImpl::FileApiCallerImpl()
{
    auto requestManager = Single<DependencyInjector>::instance().getInstance<HttpRequestManager>();
    if (!requestManager)
    {
        qFatal("HttpRequestManager instance not available");
    }

    m_visitor = std::make_shared<ApiCallVisitor>(requestManager);
}

void FileApiCallerImpl::createDirectory(const std::string& authToken, const std::string& path,
                                        const ApiCallbacks::CreateDirectoryCallback& callback) const
{
    CreateDirectoryCall call(authToken, path, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::beginUpload(const std::string& authToken, const std::string& path,
                                    const ApiCallbacks::BeginUploadCallback& callback) const
{
    BeginUploadCall call(authToken, path, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::upload(const std::string& authToken, const std::string& transferId, uint64_t offset,
                               const std::vector<uint8_t>& content, const ApiCallbacks::UploadCallback& callback) const
{
    UploadCall call(authToken, transferId, offset, content, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::beginDownload(const std::string& authToken, const std::string& path,
                                      const ApiCallbacks::BeginDownloadCallback& callback) const
{
    BeginDownloadCall call(authToken, path, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::download(const std::string& authToken, const std::string& transferId, uint64_t offset,
                                 uint64_t size, const ApiCallbacks::DownloadCallback& callback) const
{
    DownloadCall call(authToken, transferId, offset, size, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::endTransfer(const std::string& authToken, const std::string& transferId,
                                    const ApiCallbacks::EndTransferCallback& callback) const
{
    EndTransferCall call(authToken, transferId, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::getFileSize(const std::string& authToken, const std::string& path,
                                    const ApiCallbacks::GetFileSizeCallback& callback) const
{
    GetFileSizeCall call(authToken, path, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::list(const std::string& authToken, const std::string& path,
                             const ApiCallbacks::ListCallback& callback) const
{
    ListCall call(authToken, path, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::remove(const std::string& authToken, const std::string& path,
                               const ApiCallbacks::RemoveCallback& callback) const
{
    RemoveCall call(authToken, path, callback);
    call.send(m_visitor);
}

void FileApiCallerImpl::move(const std::string& authToken, const std::string& source, const std::string& destination,
                             const ApiCallbacks::MoveCallback& callback) const
{
    MoveCall call(authToken, source, destination, callback);
    call.send(m_visitor);
}
