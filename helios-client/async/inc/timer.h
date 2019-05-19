#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <cstdint>
#include <functional>

// Forward declarations
class QTimer;

/**
 * @class Timer
 * @brief QTimer wrapper
 */
class Timer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit Timer(QObject* parent = nullptr);

    /**
     * @brief Start timer
     * @param msec - Interval in milliseconds
     * @param func - Function to call on every timer shot
     * @param singleShot - Fire timer a single time
     */
    void start(uint32_t msec, std::function<void()> func, bool singleShot);

    /**
     * @brief Stop timer
     */
    void stop();

public slots:
    /**
     * @brief Called when QTimer emits a timeout singal
     */
    void onTimeout();

private:
    /**
     * @brief QTimer instance
     */
    QTimer* m_timer;

    /**
     * @brief Function to call on timeout
     */
    std::function<void()> m_func;
};

#endif  // TIMER_H
