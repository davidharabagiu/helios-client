#include "authenticatedservice.h"

bool AuthenticatedService::enabled() const
{
    return m_session.valid();
}

void AuthenticatedService::setSession(const UserSession& session)
{
    m_session = session;
}

void AuthenticatedService::removeSession()
{
    m_session.clear();
}
