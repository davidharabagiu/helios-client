#ifndef AUTHENTICATEDSERVICE_H
#define AUTHENTICATEDSERVICE_H

#include <string>

#include "usersession.h"

/**
 * @class AuthenticatedService
 * @brief Abstraction for an authenticated service i.e. a service which needs an authentication token in order to work
 */
class AuthenticatedService
{
public:
    /**
     * @brief Destructor
     */
    virtual ~AuthenticatedService() = default;

    /**
     * @brief Returns true if the service is enabled (a session is present).
     */
    virtual bool enabled() const;

    /**
     * @brief Set the authentication session of the working user. Having a valid user session is a precondition for all
     * authenticated operations to work.
     * @param session - User session containing the user name and authentication token
     */
    virtual void setSession(const UserSession& session);

    /**
     * @brief Removed the current user session. This will disable all operations.
     */
    virtual void removeSession();

protected:
    /**
     * @brief Current user session
     */
    UserSession m_session;
};

#endif  // AUTHENTICATEDSERVICE_H
