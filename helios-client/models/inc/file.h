#ifndef FILE_H
#define FILE_H

#include <string>
#include <cstdint>

/**
 * @class File
 * @brief Internal representation of a remote file together metadata and operation state information
 */
class File
{
public:
    /**
     * @brief Constructor
     * @param name - File name
     * @param parentDirectory - Parent directory path
     * @param isDirectory - Is directory ?
     * @param size - Size in bytes
     */
    File(const std::string& name, const std::string& parentDirectory, bool isDirectory, uint64_t size = 0);

    /**
     * @brief Getter for m_name
     * @return std::string
     */
    std::string name() const;

    /**
     * @brief Getter for m_fullPath
     * @return std::string
     */
    std::string fullPath() const;

    /**
     * @brief Getter for m_isDirectory
     * @return bool
     */
    bool isDirectory() const;

    /**
     * @brief Getter for m_size
     * @return uint64_t
     */
    uint64_t size() const;

    /**
     * @brief Getter for m_downloadInProgress
     * @return bool
     */
    bool downloadInProgress() const;

    /**
     * @brief Getter for m_uploadInProgress
     * @return bool
     */
    bool uploadInProgress() const;

    /**
     * @brief Getter for m_operationProgress
     * @return float
     */
    float operationProgress() const;

    /**
     * @brief Setter for m_name. m_fullPath will be changed as well.
     * @param newVal - const std::string&
     */
    void setName(const std::string& newVal);

    /**
     * @brief Setter for m_size. Works only if m_isDirectory is false.
     * @param newVal - uint64_t
     */
    void setSize(uint64_t newVal);

    /**
     * @brief Setter for m_downloadInProgress
     * @param newVal - bool
     */
    void setDownloadInProgress(bool newVal);

    /**
     * @brief Setter for m_uploadInProgress
     * @param newVal - bool
     */
    void setUploadInProgresss(bool newVal);

    /**
     * @brief Setter for m_operationProgress
     * @param newVal - float
     */
    void setOperationProgress(float newVal);

private:
    /**
     * @brief Obtain the full path using the name and parent directory.
     */
    void updateFullPath();

private:
    /**
     * @brief File name
     */
    std::string m_name;

    /**
     * @brief Parent directory of the file
     */
    std::string m_parentDirectory;

    /**
     * @brief Full path to the file
     */
    std::string m_fullPath;

    /**
     * @brief True if the file is a directory
     */
    bool m_isDirectory;

    /**
     * @brief Size of the file in bytes. Valid only if the file is not a directory and if there is no upload in progress
     */
    uint64_t m_size;

    /**
     * @brief There is a download in progress for this file
     */
    bool m_downloadInProgress;

    /**
     * @brief The is an upload in progress for this file
     */
    bool m_uploadInProgress;

    /**
     * @brief Download or upload progress (normalized). Valid only when m_downloadInProgress or m_uploadInProgress is
     * true.
     */
    float m_operationProgress;
};

#endif  // FILE_H
