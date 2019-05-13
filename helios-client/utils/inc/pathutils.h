#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <string>

/**
 * @class PathUtils
 * @brief Path manipulation utilities
 */
class PathUtils
{
public:
    /**
     * @brief Deleted constructor
     */
    PathUtils() = delete;

    /**
     * @brief Concatenate a base with a relative path to obtain the full path
     * @param base - Base path
     * @param relativePath - Relative path
     * @return Full path
     */
    static std::string concatenatePaths(const std::string& base, const std::string& relativePath);

    /**
     * @brief Get the file name and parent directory from its full path
     * @param path - Full path
     * @param name - Out param, name of the file
     * @param parent - Out param, parent directory of the file
     */
    static void getFileNameAndParentDir(const std::string& path, std::string& name, std::string& parent);

    /**
     * @brief Remove the trailing slash from a path if it exists
     * @param path - Path
     */
    static void removeTrailingSlash(std::string& path);
};

#endif  // PATHUTILS_H
