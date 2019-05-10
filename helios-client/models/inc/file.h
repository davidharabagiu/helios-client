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
     * @brief Getter for m_parentDirectory
     * @return std::string
     */
    std::string parentDirectory() const;

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
};

#endif  // FILE_H
