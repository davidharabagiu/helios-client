#ifndef SINGLE_H
#define SINGLE_H

/**
 * @class Single
 * @brief Generic singleton implementation
 * @tparam T - type to encapsulate
 */
template <typename T>
class Single
{
public:
    /**
     * @brief Returns the single instance of type T
     * @return T&
     */
    static T& instance()
    {
        static T singleInstance;
        return singleInstance;
    }

private:
    Single()              = delete;
    Single(const Single&) = delete;
    Single(Single&&)      = delete;
    Single& operator=(const Single&) = delete;
};

#endif  // SINGLE_H
