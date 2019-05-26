#include "notification.h"

Notification::Notification(const std::string& id, const std::string& text, Type type)
    : m_id(id)
    , m_text(text)
    , m_type(type)
{
}

std::string Notification::id() const
{
    return m_id;
}

std::string Notification::text() const
{
    return m_text;
}

Notification::Type Notification::type() const
{
    return m_type;
}
