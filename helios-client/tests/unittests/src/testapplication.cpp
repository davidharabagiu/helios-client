#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "testapplication.h"

const QEvent::Type TestApplication::RunTests = static_cast<QEvent::Type>(QEvent::registerEventType());

TestApplication::TestApplication(int& argc, char** argv)
    : QCoreApplication(argc, argv)
{
    testing::InitGoogleMock(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "threadsafe";
}

bool TestApplication::event(QEvent* event)
{
    if (event && event->type() == RunTests)
    {
        QCoreApplication::exit(RUN_ALL_TESTS());
    }
    return QCoreApplication::event(event);
}
