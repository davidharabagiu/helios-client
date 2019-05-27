#include "qnotification.h"

QNotification::QNotification(const std::shared_ptr<const Notification>& data)
    : m_data(data)
{
}

bool QNotification::isValid() const
{
    return static_cast<bool>(m_data);
}

QString QNotification::id() const
{
    if (m_data)
    {
        return QString::fromStdString(m_data->id());
    }
    return QString();
}

QString QNotification::text() const
{
    if (m_data)
    {
        return QString::fromStdString(m_data->text());
    }
    return QString();
}

QNotification::QNotificationType QNotification::type() const
{
    if (m_data)
    {
        return static_cast<QNotificationType>(m_data->type());
    }
    return static_cast<QNotificationType>(0);
}

std::shared_ptr<const Notification> QNotification::data() const
{
    return m_data;
}
