#include <memory>

#include "asyncnotifier.h"

AsyncNotifier::AsyncNotifier()
    : m_running(true)
    , m_executor([this] { executionLoop(); })
{
}

AsyncNotifier::~AsyncNotifier()
{
    m_running.store(false);
    cancelPendingNotifications();

    // This is done in order to prevent a deadlock in case the executor
    // is waiting for the queue to be populated
    m_notEmptyCondVar.notify_one();

    m_executor.join();
}

void AsyncNotifier::cancelPendingNotifications()
{
    std::lock_guard<std::mutex> lock(m_pendingNotificationsMutex);
    m_pendingNotifications = std::queue<std::function<void()>>();
}

void AsyncNotifier::executionLoop()
{
    while (m_running.load())
    {
        std::function<void()> notification;
        {
            std::unique_lock<std::mutex> lock(m_pendingNotificationsMutex);
            m_notEmptyCondVar.wait(lock, [this] { return !m_pendingNotifications.empty() || !m_running.load(); });

            if (m_pendingNotifications.size() > 0)
            {
                notification = m_pendingNotifications.front();
                m_pendingNotifications.pop();
            }
        }
        if (notification)
        {
            notification();
        }
    }
}
