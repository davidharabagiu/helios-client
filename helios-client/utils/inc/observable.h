#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <memory>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <utility>

#include "executor.h"

namespace __internal
{
/**
 * @class ObservableBase
 * @brief Base for both Observable class template specializations
 */
template <typename Listener>
class ObservableBase
{
public:
    /**
     * @brief Destructor
     */
    virtual ~ObservableBase() = default;

    /**
     * @brief Register a listener for receiving notifications
     * @param listener - Listener
     */
    void registerListener(const std::shared_ptr<Listener>& listener)
    {
        m_listeners.push_back(listener);
    }

    /**
     * @brief Unregister a listener from receiving notifications
     * @param listener - Listener
     */
    void unregisterListener(const std::shared_ptr<Listener>& listener)
    {
        auto it = std::find_if(m_listeners.begin(), m_listeners.end(),
                               [&listener](const std::weak_ptr<Listener>& el) { return el.lock() == listener; });
        if (it != m_listeners.end())
        {
            m_listeners.erase(it);
        }
    }

protected:
    /**
     * @brief Listeners / observers
     */
    std::vector<std::weak_ptr<Listener>> m_listeners;
};
}  // namespace __internal

/**
 * @brief Specifies how the observable should handle notification sending
 */
enum class ObservableNotifyMode
{
    /**
     * @brief Notifications will be sent to listeners immediately on the same thread.
     */
    SYNC,
    /**
     * @brief Notifications will be posted on an event queue and processes asynchronously.
     */
    ASYNC
};

template <typename Listener, ObservableNotifyMode notifyMode>
class Observable
{
    Observable()                  = delete;
    Observable(const Observable&) = delete;
    Observable(Observable&&)      = delete;
};

/**
 * @class Observable
 * @brief Generic observer pattern implementation with synchronous notifications
 * @tparam Listener - Listener / Observer type
 */
template <typename Listener>
class Observable<Listener, ObservableNotifyMode::SYNC> : public __internal::ObservableBase<Listener>
{
protected:
    /**
     * @brief notifyAll
     * @param callback - Callback method
     * @param args - Callback arguments
     * @tparam M - Callback method type
     * @tparam Args - Callback argument types
     */
    template <typename M, typename... Args>
    void notifyAll(M&& callback, Args&&... args)
    {
        auto& listeners = __internal::ObservableBase<Listener>::m_listeners;
        for (auto it = listeners.cbegin(); it != listeners.cend();)
        {
            if (it->expired())
            {
                listeners.erase(it);
            }
            else
            {
                it->lock().get()->*callback(std::forward<Args>(args)...);
                ++it;
            }
        }
    }
};

/**
 * @class Observable
 * @brief Generic observer pattern implementation with asynchronous notifications
 * @tparam Listener - Listener / Observer type
 */
template <typename Listener>
class Observable<Listener, ObservableNotifyMode::ASYNC> : public __internal::ObservableBase<Listener>
{
public:
    /**
     * @brief Constructor
     */
    Observable()
        : m_executor(new Executor())
    {
    }

protected:
    /**
     * @brief notifyAll
     * @param callback - Callback method
     * @param args - Callback arguments
     * @tparam M - Callback method type
     * @tparam Args - Callback argument types
     */
    template <typename M, typename... Args>
    void notifyAll(M&& callback, Args&&... args)
    {
        auto& listeners = __internal::ObservableBase<Listener>::m_listeners;
        for (auto it = listeners.cbegin(); it != listeners.cend();)
        {
            if (it->expired())
            {
                listeners.erase(it);
            }
            else
            {
                m_executor->post(std::forward<M>(callback), it->lock().get(), std::forward<Args>(args)...);
                ++it;
            }
        }
    }

    /**
     * @brief Cancel all posted notifications that were not yet executed.
     */
    void cancelPendingNotifications()
    {
        m_executor->cancelPendingTasks();
    }

private:
    /**
     * @brief Executor instance
     */
    std::shared_ptr<Executor> m_executor;
};

#endif  // OBSERVABLE_H
