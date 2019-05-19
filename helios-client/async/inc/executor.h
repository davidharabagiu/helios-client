#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>

/**
 * @class Executor
 * @brief Utility used to execute asynchronous tasks
 */
class Executor
{
public:
    /**
     * @brief Constructor
     */
    Executor();

    /**
     * @brief Destructor
     */
    ~Executor();

    /**
     * @brief post
     * @param callback - Completion callback
     * @param args - Completion callback arguments
     * @tparam F - Completion callback type
     * @tparam Args - Completion callback arguments types
     */
    template <typename F, typename... Args>
    void post(F&& callback, Args&&... args);

    /**
     * @brief Cancel all pending tasks
     */
    void cancelPendingTasks();

private:
    /**
     * @brief Execution loop of the executor thread
     */
    void executionLoop();

private:
    /**
     * @brief Pending tasks
     */
    std::queue<std::function<void()>> m_pendingTasks;

    /**
     * @brief Mutex for pending tasks queue
     */
    std::mutex m_pendingTasksMutex;

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

#include "executor.tpp"

#endif  // EXECUTOR_H
