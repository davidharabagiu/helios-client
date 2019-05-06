#ifndef TYPEUTILS_H
#define TYPEUTILS_H

#include <string>
#include <cxxabi.h>
#include <typeinfo>
#include <QDebug>

namespace TypeUtils
{
template <typename T>
std::string getTypeName()
{
    int         status;
    const char* abiName = typeid(T).name();
    const char* pName   = abi::__cxa_demangle(abiName, 0, 0, &status);

    if (status != 0 || !pName)
    {
        qCritical() << "Failed to demangle name " << abiName;
    }

    std::string name(pName);
    delete pName;

    return name;
}
}  // namespace TypeUtils

#endif  // TYPEUTILS_H
