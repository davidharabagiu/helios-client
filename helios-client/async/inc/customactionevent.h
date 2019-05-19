#ifndef CUSTOMACTIONEVENT_H
#define CUSTOMACTIONEVENT_H

#include <QEvent>
#include <functional>

/**
 * @class CustomActionEvent
 * @brief Custom QEvent for executing an arbitraty action
 */
class CustomActionEvent : public QEvent
{
public:
    /**
     * @brief CustomActionEvent
     * @param action - Action
     * @param eventType - Event type
     */
    CustomActionEvent(std::function<void()> action, QEvent::Type eventType);

    /**
     * @brief Execute action
     */
    void operator()() const;

private:
    /**
     * @brief Action
     */
    std::function<void()> m_action;
};

#endif  // CUSTOMACTIONEVENT_H
