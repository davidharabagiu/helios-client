#include "asyncnotifier.h"

template <typename F, typename... Args>
void AsyncNotifier::postNotification(F&& callback, Args&&... args)
{
    {
        std::lock_guard<std::mutex> lock(m_pendingNotificationsMutex);
        m_pendingNotifications.push(std::bind(std::forward<F>(callback), std::forward<Args>(args)...));
    }
    m_notEmptyCondVar.notify_one();
}
