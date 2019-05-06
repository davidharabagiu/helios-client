#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <memory>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <utility>

#include "asyncnotifier.h"

namespace
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
        auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
        if (it != m_listeners.end())
        {
            m_listeners.erase(it);
        }
    }

protected:
    /**
     * @brief Listeners / observers
     */
    std::vector<std::shared_ptr<Listener>> m_listeners;
};
}  // namespace

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
class Observable<Listener, ObservableNotifyMode::SYNC> : public ObservableBase<Listener>
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
        for (const auto& listener : ObservableBase<Listener>::m_listeners)
        {
            listener.get()->*callback(std::forward<Args>(args)...);
        }
    }
};

/**
 * @class Observable
 * @brief Generic observer pattern implementation with asynchronous notifications
 * @tparam Listener - Listener / Observer type
 */
template <typename Listener>
class Observable<Listener, ObservableNotifyMode::ASYNC> : public ObservableBase<Listener>
{
public:
    /**
     * @brief Constructor
     */
    Observable()
        : m_asyncNotifier(new AsyncNotifier())
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
        for (const auto& listener : ObservableBase<Listener>::m_listeners)
        {
            m_asyncNotifier->postNotification(std::forward<M>(callback), listener.get(), std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Cancel all posted notifications that were not yet executed. Only works when notifyMode is ASYNC.
     */
    void cancelPendingNotifications()
    {
        m_asyncNotifier->cancelPendingNotifications();
    }

private:
    /**
     * @brief Async notifier instance (may be null in case m_notifyMode is SYNC)
     */
    std::shared_ptr<AsyncNotifier> m_asyncNotifier;
};

#endif  // OBSERVABLE_H
