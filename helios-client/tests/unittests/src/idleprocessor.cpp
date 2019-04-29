#include <QElapsedTimer>
#include <QCoreApplication>
#include <thread>

#include "idleprocessor.h"

void IdleProcessor::processEventsUntilTimeout(int64_t timeout, bool yieldThread)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < timeout)
    {
        QCoreApplication::processEvents();
        if (yieldThread)
        {
            std::this_thread::yield();
        }
    }
}

void IdleProcessor::processEventsUntilPredicate(const std::function<bool()>& predicate, int64_t timeout,
                                                bool yieldThread)
{
    QElapsedTimer timer;
    timer.start();
    while ((timeout < 0 || timer.elapsed() < timeout) && !predicate())
    {
        QCoreApplication::processEvents();
        if (yieldThread)
        {
            std::this_thread::yield();
        }
    }
}
