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
                              const ApiCallbacks::RegisterUserCallback& callback) const = 0;

    /**
     * @brief Authenticate as an existing user.
     * @param username - Username
     * @param password - Password
     * @param callback - LoginCallback
     */
    virtual void login(const std::string& username, const std::string& password,
                       const ApiCallbacks::LoginCallback& callback) const = 0;

    /**
     * @brief Logout from the system using an active authentication token.
     * @param token - Authentication token
     * @param callback - LogoutCallback
     */
    virtual void logout(const std::string& token, const ApiCallbacks::LogoutCallback& callback) const = 0;

    /**
     * @brief Check the validity of a token and if it corresponds with the given username.
     * @param username - User name
     * @param token - Authentication token
     * @param callback - CheckTokenCallback
     */
    virtual void checkToken(const std::string& username, const std::string& token,
                            const ApiCallbacks::CheckTokenCallback& callback) const = 0;

    /**
     * @brief Get the public key of a user
     * @param token - Authentication token
     * @param username - Username of the key owner
     * @param callback - GetUserKeyCallback
     */
    virtual void getUserKey(const std::string& token, const std::string& username,
                            const ApiCallbacks::GetUserKeyCallback& callback) const = 0;
};

#endif  // USERAPICALLER_H
