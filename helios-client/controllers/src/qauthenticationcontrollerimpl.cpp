#include <QDebug>

#include "qauthenticationcontrollerimpl.h"
#include "qauthenticationcontroller.h"
#include "single.h"
#include "dependencyinjector.h"
#include "userservice.h"
#include "useraccount.h"
#include "usersession.h"

QAuthenticationControllerImpl::QAuthenticationControllerImpl(QAuthenticationController* publicImpl)
    : m_publicImpl(publicImpl)
{
    m_userService = Single<DependencyInjector>::instance().getInstance<UserService>();
    if (!m_userService)
    {
        qFatal("UserService instance not available");
    }

    if (!m_userService->enabled())
    {
        qFatal("UserService is disabled");
    }
}

void QAuthenticationControllerImpl::registerForNotifications()
{
    m_userService->registerListener(shared_from_this());
}

void QAuthenticationControllerImpl::unregisterFromNotifications()
{
    m_userService->unregisterListener(shared_from_this());
}

void QAuthenticationControllerImpl::loginCompleted(bool success, const std::string& errorString)
{
    if (success)
    {
        QMetaObject::invokeMethod(m_publicImpl, "loggedInChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "usernameChanged", Qt::QueuedConnection);
    }

    QMetaObject::invokeMethod(m_publicImpl, "loginCompleted", Qt::QueuedConnection, Q_ARG(bool, success),
                              Q_ARG(const QString&, QString::fromStdString(errorString)));
}

void QAuthenticationControllerImpl::logoutCompleted(bool success, const std::string& errorString)
{
    if (success)
    {
        QMetaObject::invokeMethod(m_publicImpl, "loggedInChanged", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_publicImpl, "usernameChanged", Qt::QueuedConnection);
    }

    QMetaObject::invokeMethod(m_publicImpl, "logoutCompleted", Qt::QueuedConnection, Q_ARG(bool, success),
                              Q_ARG(const QString&, QString::fromStdString(errorString)));
}

void QAuthenticationControllerImpl::userCreationCompleted(bool success, const std::string& errorString)
{
    QMetaObject::invokeMethod(m_publicImpl, "userCreationCompleted", Qt::QueuedConnection, Q_ARG(bool, success),
                              Q_ARG(const QString&, QString::fromStdString(errorString)));
}

void QAuthenticationControllerImpl::restoreSession()
{
    m_userService->restoreSession();
}

bool QAuthenticationControllerImpl::login(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty())
    {
        return false;
    }

    m_userService->login(UserAccount(username.toStdString(), password.toStdString()), m_persistLogin);
    return true;
}

bool QAuthenticationControllerImpl::logout()
{
    if (m_userService->loggedIn())
    {
        m_userService->logout();
        return true;
    }

    return false;
}

bool QAuthenticationControllerImpl::createUser(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty())
    {
        return false;
    }

    m_userService->createUser(UserAccount(username.toStdString(), password.toStdString()));
    return true;
}

void QAuthenticationControllerImpl::setPersistLogin(bool newVal)
{
    m_persistLogin = newVal;
}

bool QAuthenticationControllerImpl::loggedIn() const
{
    return m_userService->loggedIn();
}

QString QAuthenticationControllerImpl::username() const
{
    if (m_userService->loggedIn())
    {
        return QString::fromStdString(m_userService->session().username());
    }
    return QString();
}
