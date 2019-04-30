#ifndef TESTAPPLICATION_H
#define TESTAPPLICATION_H

#include <QCoreApplication>
#include <QEvent>
#include <cstdint>
#include <functional>

/**
 * @class TestApplication
 * @brief Test application which supports classes that require Qt event loop
 */
class TestApplication : public QCoreApplication
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param argc - Number of arguments
     * @param argv - Application arguments array
     */
    TestApplication(int& argc, char** argv);

    static void processEventsUntilTimeout(int64_t timeout, bool yieldThread = true);

    static void processEventsUntilPredicate(const std::function<bool()>& predicate, int64_t timeout = -1,
                                            bool yieldThread = true);

protected:  // from QCoreApplication
    bool event(QEvent* event) override;

public:
    /**
     * @brief Event type for running tests
     */
    static const QEvent::Type RunTests;
};

#endif  // TESTAPPLICATION_H
