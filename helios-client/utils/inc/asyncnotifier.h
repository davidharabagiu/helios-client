#ifndef ASYNCNOTIFIER_H
#define ASYNCNOTIFIER_H

#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>

/**
 * @class AsyncNotifiser
 * @brief Utility used to notify clients asynchronously
 */
class AsyncNotifier
{
public:
    /**
     * @brief Constructor
     */
    AsyncNotifier();

    /**
     * @brief Destructor
     */
    ~AsyncNotifier();

    /**
     * @brief postNotification
     * @param callback - Notification callback
     * @param args - Notification callback arguments
     * @tparam F - Notification callback type
     * @tparam Args - Notification callback arguments types
     */
    template <typename F, typename... Args>
    void postNotification(F&& callback, Args&&... args);

    /**
     * @brief Cancel all pending notifications
     */
    void cancelPendingNotifications();

private:
    /**
     * @brief Execution loop of the executor thread
     */
    void executionLoop();

private:
    /**
     * @brief Pending notifications
     */
    std::queue<std::function<void()>> m_pendingNotifications;

    /**
     * @brief Mutex for pending notifications queue
     */
    std::mutex m_pendingNotificationsMutex;

    /**
     * @brief Condition variable for waiting for the queue to be populated
     */
    std::condition_variable m_notEmptyCondVar;

    /**
     * @brief The executor loop will run as long as this is true
     */
    std::atomic_bool m_running;

    /**
     * @brief Executor
     */
    std::thread m_executor;
};

#include "asyncnotifier.tpp"

#endif  // ASYNCNOTIFIER_H
