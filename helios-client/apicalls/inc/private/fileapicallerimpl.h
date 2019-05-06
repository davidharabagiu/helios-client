#ifndef FILEAPICALLERIMPL_H
#define FILEAPICALLERIMPL_H

#include <memory>

#include "fileapicaller.h"

class ApiCallVisitor;
class HttpRequestManager;

/**
 * @class FileApiCallerImpl
 * @brief Private implementation of FileApiCaller
 */
class FileApiCallerImpl : public FileApiCaller
{
public:
    /**
     * @brief Constructor
     * @param requestManager - Http request manager instance
     */
    FileApiCallerImpl(const std::shared_ptr<HttpRequestManager>& requestManager);

public:  // from FileApiCaller
    void createDirectory(const std::string& authToken, const std::string& path,
                         const ApiCallbacks::CreateDirectoryCallback& callback) const override;
    void beginUpload(const std::string& authToken, const std::string& path,
                     const ApiCallbacks::BeginUploadCallback& callback) const override;
    void upload(const std::string& authToken, const std::string& transferId, uint64_t offset,
                const std::vector<uint8_t>& content, const ApiCallbacks::UploadCallback& callback) const override;
    void beginDownload(const std::string& authToken, const std::string& path,
                       const ApiCallbacks::BeginDownloadCallback& callback) const override;
    void download(const std::string& authToken, const std::string& transferId, uint64_t offset, uint64_t size,
                  const ApiCallbacks::DownloadCallback& callback) const override;
    void endTransfer(const std::string& authToken, const std::string& transferId,
                     const ApiCallbacks::EndTransferCallback& callback) const override;
    void getFileSize(const std::string& authToken, const std::string& path,
                     const ApiCallbacks::GetFileSizeCallback& callback) const override;
    void list(const std::string& authToken, const std::string& path,
              const ApiCallbacks::ListCallback& callback) const override;
    void remove(const std::string& authToken, const std::string& path,
                const ApiCallbacks::RemoveCallback& callback) const override;
    void move(const std::string& authToken, const std::string& source, const std::string& destination,
              const ApiCallbacks::MoveCallback& callback) const override;

private:
    /**
     * @brief API call visitor
     */
    std::shared_ptr<ApiCallVisitor> m_visitor;
};

#endif  // FILEAPICALLERIMPL_H
