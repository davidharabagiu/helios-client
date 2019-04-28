#ifndef TYPECONVERSIONS_H
#define TYPECONVERSIONS_H

#include <memory>
#include <limits>
#include <type_traits>
#include <cassert>

/**
 * @brief Cast an unique_ptr from one type to another without runtime type checking. The object will be moved in the
 * result pointer and the original pointer will be invalid.
 * @param r - Source pointer
 * @tparam T - Result type
 * @tparam U - Source type
 * @return Result
 */
template <typename T, typename U>
inline std::unique_ptr<T> static_unique_cast(std::unique_ptr<U>&& r)
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
inline std::unique_ptr<T> dynamic_unique_cast(std::unique_ptr<U>&& r)
{
    std::unique_ptr<T> l(dynamic_cast<T*>(r.get()));
    if (l)
    {
        r.release();
    }
    return l;
}

/**
 * @brief Safe cast between two integral types which have the same signedness.
 * @param r - Source number
 * @tparam T - Result type
 * @tparam U - Source type
 * @return Result
 */
template <typename T, typename U>
inline std::enable_if_t<(std::is_signed<T>::value && std::is_signed<U>::value) ||
                            (std::is_unsigned<T>::value && std::is_unsigned<U>::value),
                        T>
safe_integral_cast(U r)
{
    if constexpr (sizeof(T) < sizeof(U))
    {
        assert(r <= static_cast<U>(std::numeric_limits<T>::max()));
        assert(r >= static_cast<U>(std::numeric_limits<T>::min()));
    }
    return static_cast<T>(r);
}

/**
 * @brief Safe cast between two integral types where the source is unsigned and the result is signed.
 * @param r - Source number
 * @tparam T - Result type
 * @tparam U - Source type
 * @return Result
 */
template <typename T, typename U>
inline std::enable_if_t<std::is_signed_v<T> && std::is_unsigned_v<U>, T> safe_integral_cast(U r)
{
    assert(r <= static_cast<U>(std::numeric_limits<std::make_signed_t<U>>::max()));
    return safe_integral_cast<T>(static_cast<std::make_signed_t<U>>(r));
}

/**
 * @brief Safe cast between two integral types where the source is signed and the result is unsigned.
 * @param r - Source number
 * @tparam T - Result type
 * @tparam U - Source type
 * @return Result
 */
template <typename T, typename U>
inline std::enable_if_t<std::is_unsigned_v<T> && std::is_signed_v<U>, T> safe_integral_cast(U r)
{
    assert(r >= 0);
    return safe_integral_cast<T>(static_cast<std::make_unsigned_t<U>>(r));
}

#endif  // TYPECONVERSIONS_H
