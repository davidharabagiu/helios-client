#include "qauthenticationcontroller.h"
#include "qauthenticationcontrollerimpl.h"

QAuthenticationController::QAuthenticationController(QObject* parent)
    : QObject(parent)
    , m_privateImpl(new QAuthenticationControllerImpl(this))
{
    m_privateImpl->registerForNotifications();
}

QAuthenticationController::~QAuthenticationController()
{
    m_privateImpl->unregisterFromNotifications();
}

void QAuthenticationController::restoreSession()
{
    m_privateImpl->restoreSession();
}

bool QAuthenticationController::login(const QString& username, const QString& password)
{
    return m_privateImpl->login(username, password);
}

bool QAuthenticationController::logout()
{
    return m_privateImpl->logout();
}

bool QAuthenticationController::createUser(const QString& username, const QString& password)
{
    return m_privateImpl->createUser(username, password);
}

void QAuthenticationController::setPersistLogin(bool newVal)
{
    m_privateImpl->setPersistLogin(newVal);
}

bool QAuthenticationController::loggedIn() const
{
    return m_privateImpl->loggedIn();
}

QUserSession QAuthenticationController::session() const
{
    return m_privateImpl->session();
}
