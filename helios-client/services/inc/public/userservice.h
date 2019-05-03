#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <string>
#include <memory>

#include "serviceinterface.h"

class UserServiceListener;

/**
 * @class UserService
 * @brief User management service
 */
class UserService : public ServiceInterface
{
public:
    /**
     * @brief Destructor
     */
    virtual ~UserService() = default;

    /**
     * @brief Register a listener for user service operations
     * @param listener - Listener
     */
    virtual void registerListener(const std::shared_ptr<UserServiceListener>& listener) = 0;

    /**
     * @brief Unregister a listener
     * @param listener - Listener
     */
    virtual void unregisterListener(const std::shared_ptr<UserServiceListener>& listener) = 0;

    /**
     * @brief Returns the current logged in user name. Valid only when logged in.
     * @return const std::string&
     */
    virtual const std::string& username() const = 0;

    /**
     * @brief Returns the current logged in user authentication token. Valid only when logged in.
     * @return const std::string&
     */
    virtual const std::string& authenticationToken() const = 0;

    /**
     * @brief Authenticate as a user
     * @param username - User name
     * @param password - User password
     * @param persist - Set to true in order to persist the authentication. Next time the application is started the
     * user will still be logged in.
     */
    virtual void login(const std::string& username, const std::string& password, bool persist) = 0;

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
     * @param username - User name
     * @param password - User password
     */
    virtual void createUser(const std::string& username, const std::string& password) = 0;
};

#endif  // USERSERVICE_H
