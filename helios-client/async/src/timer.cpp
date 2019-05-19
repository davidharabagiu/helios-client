#include <QTimer>

#include "timer.h"
#include "typeconversions.h"

Timer::Timer(QObject* parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void Timer::start(uint32_t msec, std::function<void()> func, bool singleShot)
{
    if (msec < 0 || !func)
    {
        return;
    }

    if (m_timer->isActive())
    {
        stop();
    }

    m_timer->setSingleShot(singleShot);
    m_timer->setInterval(safe_integral_cast<int>(msec));
    m_func = func;
    m_timer->start();
}

void Timer::stop()
{
    m_timer->stop();
}

void Timer::onTimeout()
{
    m_func();
}
