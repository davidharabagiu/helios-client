#ifndef MANUALRESETEVENT_H
#define MANUALRESETEVENT_H

#include <mutex>
#include <condition_variable>

/**
 * @class ManualResetEvent
 * @brief Thread syncrhonization event that, when signaled, releases a waiting thread and must be resetted manually
 */
class ManualResetEvent
{
public:
    /**
     * @brief Condition variable
     */
    ManualResetEvent();

    /**
     * @brief Signal the waiting thread
     */
    void set();

    /**
     * @brief Reset to initial state
     */
    void reset();

    /**
     * @brief Wait for signal
     */
    void wait();

private:
    std::mutex              m_mutex;
    std::condition_variable m_condVar;
    bool                    m_signaled;
};

#endif  // MANUALRESETEVENT_H
