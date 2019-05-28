#include "qusersession.h"
#include "usersession.h"

QUserSession::QUserSession(const std::shared_ptr<const UserSession>& data)
    : m_data(data)
{
}

bool QUserSession::isValid() const
{
    if (m_data)
    {
        return m_data->valid();
    }
    return false;
}

QString QUserSession::username() const
{
    if (m_data)
    {
        return QString::fromStdString(m_data->username());
    }
    return QString();
}

QString QUserSession::authToken() const
{
    if (m_data)
    {
        return QString::fromStdString(m_data->authToken());
    }
    return QString();
}

std::shared_ptr<const UserSession> QUserSession::data() const
{
    return m_data;
}
