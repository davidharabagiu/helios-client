#include <memory>

#include "useraccount.h"

UserAccount::UserAccount(const std::string& username, const std::string& password)
    : m_username(username)
    , m_password(password)
{
}

bool UserAccount::operator==(const UserAccount& rhs) const
{
    return m_username == rhs.m_username && m_password == rhs.m_password;
}

bool UserAccount::operator!=(const UserAccount& rhs) const
{
    return !(*this == rhs);
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

void test()
{
    UserAccount a1("pula", "pizda");
    UserAccount a2("cacat", "pisat");
    UserAccount a3(a1);
}
