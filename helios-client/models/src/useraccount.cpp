#include "useraccount.h"

UserAccount::UserAccount(const std::string& username, const std::string& password)
    : m_username(username)
    , m_password(password)
{
}

const std::string& UserAccount::username() const
{
    return m_username;
}

void UserAccount::setUsername(const std::string& newVal)
{
    m_username = newVal;
}

const std::string& UserAccount::password() const
{
    return m_password;
}

void UserAccount::setPassword(const std::string& newVal)
{
    m_password = newVal;
}
