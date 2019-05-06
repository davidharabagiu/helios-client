#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <string>
#include <memory>

#include "serviceinterface.h"
#include "observable.h"

class UserServiceListener;
class UserAccount;
class UserSession;

/**
 * @class UserService
 * @brief User management service
 */
class UserService : public ServiceInterface, public Observable<UserServiceListener, ObservableNotifyMode::ASYNC>
{
public:
    /**
     * @brief Destructor
     */
    virtual ~UserService() = default;

    /**
     * @brief Returns the current user session which contains the username and the authentication token. Valid only when
     * logged in.
     * @return const UserSession&
     */
    virtual const UserSession& session() const = 0;

    /**
     * @brief Logged in check
     * @return bool
     */
    virtual bool loggedIn() const = 0;

    /**
     * @brief Authenticate as a user
     * @param account - UserAccount
     * @param persist - Set to true in order to persist the authentication. Next time the application is started the
     * user will still be logged in.
     */
    virtual void login(const UserAccount& account, bool persist) = 0;

    /**
     * @brief Restore the last session if the login operation was persisted.
     */
    virtual void restoreSession() = 0;

    /**
     * @brief Log out from the system. The authentication token will become invalid and be removed.
     */
    virtual void logout() = 0;

    /**
     * @brief Create a new user
     * @param account - UserAccount
     */
    virtual void createUser(const UserAccount& account) = 0;
};

#endif  // USERSERVICE_H
