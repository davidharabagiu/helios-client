#include "manualresetevent.h"

ManualResetEvent::ManualResetEvent()
    : m_signaled(false)
{
}

void ManualResetEvent::set()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_signaled = true;
    }
    m_condVar.notify_one();
}

void ManualResetEvent::reset()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_signaled = false;
    }
}

void ManualResetEvent::wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condVar.wait(lock, [this] { return m_signaled; });
}
