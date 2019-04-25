#ifndef USERAPICALLER_H
#define USERAPICALLER_H

#include <string>
#include <functional>

#include "apicalldefs.h"

/**
 * @class UserApiCaller
 * @brief Provides calls for the user management API
 */
class UserApiCaller
{
public:
    /**
     * @brief Destructor
     */
    virtual ~UserApiCaller() = default;

    /**
     * @brief Create a new user account.
     * @param username - Name of the new user account
     * @param password - Password of the new user account
     * @param callback - RegisterUserCallback
     */
    virtual void registerUser(const std::string& username, const std::string& password,
                              const ApiCallbacks::RegisterUserCallback& callback) = 0;

    /**
     * @brief Authenticate as an existing user.
     * @param username - Username
     * @param password - Password
     * @param callback - LoginCallback
     */
    virtual void login(const std::string& username, const std::string& password,
                       const ApiCallbacks::LoginCallback& callback) = 0;

    /**
     * @brief Logout from the system using an active authentication token.
     * @param token - Authentication token
     * @param callback - LogoutCallback
     */
    virtual void logout(const std::string& token, const ApiCallbacks::LogoutCallback& callback) = 0;
};

#endif  // USERAPICALLER_H
