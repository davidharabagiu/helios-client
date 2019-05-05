#include "quseraccount.h"
#include "useraccount.h"

QUserAccount::QUserAccount()
    : m_data(new UserAccount())
{
}

QUserAccount::QUserAccount(const std::shared_ptr<UserAccount>& data)
    : m_data(data)
{
}

QString QUserAccount::username() const
{
    return QString::fromStdString(m_data->username());
}

void QUserAccount::setUsername(const QString& newVal)
{
    m_data->setUsername(newVal.toStdString());
}

QString QUserAccount::password() const
{
    return QString::fromStdString(m_data->password());
}

void QUserAccount::setPassword(const QString& newVal)
{
    m_data->setPassword(newVal.toStdString());
}

std::shared_ptr<UserAccount> QUserAccount::data()
{
    return m_data;
}
