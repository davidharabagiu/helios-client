#include "autoresetevent.h"

AutoResetEvent::AutoResetEvent()
    : m_signaled(false)
{
}

void AutoResetEvent::set()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_signaled = true;
    }
    m_condVar.notify_one();
}

void AutoResetEvent::wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condVar.wait(lock, [this] {
        bool shouldUnblock = m_signaled;
        if (shouldUnblock)
        {
            m_signaled = false;
        }
        return shouldUnblock;
    });
}
