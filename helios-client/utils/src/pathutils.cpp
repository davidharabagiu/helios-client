#include <sstream>

#include "pathutils.h"

std::string PathUtils::concatenatePaths(const std::string& base, const std::string& relativePath)
{
    std::ostringstream sstream;
    sstream << base;

    if (base.length() > 0 && base[base.length() - 1] != '/')
    {
        sstream << '/';
    }

    std::string relative(relativePath);
    removeTrailingSlash(relative);
    sstream << relative;

    return sstream.str();
}

void PathUtils::getFileNameAndParentDir(const std::string& path, std::string& name, std::string& parent)
{
    auto idx = path.rfind('/', path.length() - 2);
    if (idx != std::string::npos)
    {
        parent = path.substr(0, idx);
        name   = path.substr(idx + 1);
    }
    else
    {
        parent = "";
        name   = path;
    }
}

void PathUtils::removeTrailingSlash(std::string& path)
{
    if (path.length() > 0 && path[path.length() - 1] == '/')
    {
        path.erase(path.length() - 1);
    }
}
