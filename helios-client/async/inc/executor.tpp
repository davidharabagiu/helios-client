#include "executor.h"

template <typename F, typename... Args>
void Executor::post(F&& callback, Args&&... args)
{
    {
        std::lock_guard<std::mutex> lock(m_pendingTasksMutex);
        m_pendingTasks.push(std::bind(std::forward<F>(callback), std::forward<Args>(args)...));
    }
    m_notEmptyCondVar.notify_one();
}
