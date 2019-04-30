#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QElapsedTimer>
#include <thread>
#include <exception>
#include <QDebug>
#include <QException>

#include "testapplication.h"

const QEvent::Type TestApplication::RunTests = static_cast<QEvent::Type>(QEvent::registerEventType());

TestApplication::TestApplication(int& argc, char** argv)
    : QCoreApplication(argc, argv)
{
    testing::InitGoogleMock(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "threadsafe";
}

void TestApplication::processEventsUntilTimeout(int64_t timeout, bool yieldThread)
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

void TestApplication::processEventsUntilPredicate(const std::function<bool()>& predicate, int64_t timeout,
                                                  bool yieldThread)
{
    QElapsedTimer timer;
    timer.start();
    while ((timeout < 0 || timer.elapsed() < timeout) && !predicate())
    {
        try
        {
            QCoreApplication::processEvents();
        }
        catch (...)
        {
            qDebug() << "Caught exception while processing events";
        }

        if (yieldThread)
        {
            std::this_thread::yield();
        }
    }
}

bool TestApplication::event(QEvent* event)
{
    if (event && event->type() == RunTests)
    {
        QCoreApplication::exit(RUN_ALL_TESTS());
    }
    return QCoreApplication::event(event);
}
