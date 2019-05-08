#ifndef FILESERVICEIMPL_H
#define FILESERVICEIMPL_H

#include "fileservice.h"
#include "fileapicaller.h"

/**
 * @class FileServiceImpl
 * @brief Private implementation of FileService
 */
class FileServiceImpl : public FileService
{
public:
    /**
     * @brief Constructor
     * @param fileApiCaller - File API caller instance
     */
    FileServiceImpl(std::unique_ptr<FileApiCaller> fileApiCaller);

public:  // from FileService
    void              setAuthToken(const std::string& authToken);
    void              removeAuthToken();
    std::string       currentDirectory() const;
    std::vector<File> files() const;
    void              changeCurrentDirectory(const std::string& path);
    void              createDirectory(const std::string& name);
    void              uploadFile(const std::string& localPath);
    void              downloadFile(const std::string& name, const std::string& localPath);
    void              cancelOperation(const std::string& filePath);
    void              moveFile(const std::string& name, const std::string& newParentDirectory);
    void              renameFile(const std::string& name, const std::string& newName);
    void              removeFile(const std::string& name);

private:
    /**
     * @brief File operations API caller instance
     */
    std::unique_ptr<FileApiCaller> m_apiCaller;
};

#endif  // FILESERVICEIMPL_H
