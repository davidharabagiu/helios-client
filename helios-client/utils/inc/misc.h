#ifndef MISC_H
#define MISC_H

#include <memory>

/**
 * @brief Cast an unique_ptr from one type to another without runtime type checking. The object will be moved in the
 * result pointer and the original pointer will be invalid.
 * @param r - Source pointer
 * @tparam T - Result type
 * @tparam U - Source type
 * @return Result
 */
template <typename T, typename U>
std::unique_ptr<T> static_unique_cast(std::unique_ptr<U>&& r)
{
    std::unique_ptr<T> l(static_cast<T*>(r.get()));
    r.release();
    return l;
}

/**
 * @brief Cast an unique_ptr from one type to another. If the destination type is compatible with the object, then the
 * object will be moved int the result pointer and the source pointer will be invalid. If the destination type is not
 * compatible, the source will remain unchanged and an invalid pointer will be returned.
 * @param r - Source pointer
 * @tparam T - Result type
 * @tparam U - Source type
 * @return Result
 */
template <typename T, typename U>
std::unique_ptr<T> dynamic_unique_cast(std::unique_ptr<U>&& r)
{
    std::unique_ptr<T> l(dynamic_cast<T*>(r.get()));
    if (l)
    {
        r.release();
    }
    return l;
}

#endif  // MISC_H
