#ifndef AUTORESETEVENT_H
#define AUTORESETEVENT_H

#include <mutex>
#include <condition_variable>

/**
 * @class AutoResetEvent
 * @brief Thread syncrhonization event that, when signaled, releases a waiting thread and resets automatically
 */
class AutoResetEvent
{
public:
    /**
     * @brief Condition variable
     */
    AutoResetEvent();

    /**
     * @brief Signal the waiting thread
     */
    void set();

    /**
     * @brief Wait for signal
     */
    void wait();

private:
    std::mutex              m_mutex;
    std::condition_variable m_condVar;
    bool                    m_signaled;
};

#endif  // AUTORESETEVENT_H
