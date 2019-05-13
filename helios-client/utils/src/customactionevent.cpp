#include "customactionevent.h"

CustomActionEvent::CustomActionEvent(std::function<void()> action, QEvent::Type eventType)
    : QEvent(eventType)
    , m_action(action)
{
}

void CustomActionEvent::operator()() const
{
    m_action();
}
