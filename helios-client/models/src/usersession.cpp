#include "usersession.h"

UserSession::UserSession(const std::string& username, const std::string& authToken)
    : m_username(username)
    , m_authToken(authToken)
{
}

bool UserSession::operator==(const UserSession& rhs) const
{
    return m_username == rhs.m_username && m_authToken == rhs.m_authToken;
}

bool UserSession::operator!=(const UserSession& rhs) const
{
    return !(*this == rhs);
}

void UserSession::clear()
{
    m_username.clear();
    m_authToken.clear();
}

bool UserSession::valid() const
{
    return !m_username.empty() && !m_authToken.empty();
}

const std::string& UserSession::username() const
{
    return m_username;
}

void UserSession::setUsername(const std::string& newVal)
{
    m_username = newVal;
}

const std::string& UserSession::authToken() const
{
    return m_authToken;
}

void UserSession::setAuthToken(const std::string& newVal)
{
    m_authToken = newVal;
}
