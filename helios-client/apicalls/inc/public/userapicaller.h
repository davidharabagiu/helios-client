#ifndef USERAPICALLER_H
#define USERAPICALLER_H

#include <string>
#include <functional>

#include "apicallstatus.h"

/**
 * @class UserApiCaller
 * @brief Provides calls for the user management API
 */
class UserApiCaller
{
public:  // callback definitions
    /**
     * @brief Register user callback function. Receives the call status.
     */
    using RegisterUserCallback = std::function<void(ApiCallStatus)>;

    /**
     * @brief Login callback function. Receives the call status and the authentication token.
     */
    using LoginCallback = std::function<void(ApiCallStatus, const std::string&)>;

    /**
     * @brief Logout callback function. Receives the call status.
     */
    using LogoutCallback = std::function<void(ApiCallStatus)>;

public:
    /**
     * @brief Create a new user account.
     * @param username - Name of the new user account
     * @param password - Password of the new user account
     * @param callback - RegisterUserCallback
     */
    void registerUser(const std::string& username, const std::string& password, const RegisterUserCallback& callback);

    /**
     * @brief Authenticate as an existing user.
     * @param username - Username
     * @param password - Password
     * @param callback - LoginCallback
     */
    void login(const std::string& username, const std::string& password, const LoginCallback& callback);

    /**
     * @brief Logout from the system using an active authentication token.
     * @param token - Authentication token
     * @param callback - LogoutCallback
     */
    void logout(const std::string& token, const LogoutCallback& callback);
};

#endif  // USERAPICALLER_H
