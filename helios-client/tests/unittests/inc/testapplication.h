#ifndef TESTAPPLICATION_H
#define TESTAPPLICATION_H

#include <QCoreApplication>
#include <QEvent>

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

protected:  // from QCoreApplication
    bool event(QEvent* event) override;

public:
    /**
     * @brief Event type for running tests
     */
    static const QEvent::Type RunTests;
};

#endif  // TESTAPPLICATION_H
