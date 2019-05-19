#include <memory>

#include "executor.h"

Executor::Executor()
    : m_running(true)
    , m_executor([this] { executionLoop(); })
{
}

Executor::~Executor()
{
    m_running.store(false);
    cancelPendingTasks();

    // This is done in order to prevent a deadlock in case the executor
    // is waiting for the queue to be populated
    m_notEmptyCondVar.notify_one();
    m_executor.join();
}

void Executor::cancelPendingTasks()
{
    std::lock_guard<std::mutex> lock(m_pendingTasksMutex);
    m_pendingTasks = std::queue<std::function<void()>>();
}

void Executor::executionLoop()
{
    while (m_running.load())
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_pendingTasksMutex);
            m_notEmptyCondVar.wait(lock, [this] { return !m_pendingTasks.empty() || !m_running.load(); });

            if (m_pendingTasks.size() > 0)
            {
                task = m_pendingTasks.front();
                m_pendingTasks.pop();
            }
        }
        if (task)
        {
            task();
        }
    }
}
