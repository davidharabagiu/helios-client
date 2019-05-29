#ifndef USERSERVICEIMPL_H
#define USERSERVICEIMPL_H

#include <memory>
#include <string>
#include <vector>

#include "userservice.h"
#include "apicalldefs.h"
#include "usersession.h"
#include "executor.h"

// Forward declarations
class UserApiCaller;
class SettingsManager;
class Rsa;
class KeyManager;

/**
 * @class UserServiceImpl
 * @brief Private implementation of UserService
 */
class UserServiceImpl : public UserService
{
public:
    /**
     * @brief Constructor
     * @param userApiCaller - Api caller for user operations
     * @param settingsManager - Settings manager
     * @param rsa - Rsa utilities instance
     * @param keyManager - Key manager instance
     */
    UserServiceImpl(std::unique_ptr<UserApiCaller>          userApiCaller,
                    const std::shared_ptr<SettingsManager>& settingsManager, std::unique_ptr<Rsa> rsa,
                    const std::shared_ptr<KeyManager>& keyManager);

public:  // from UserService
    const UserSession& session() const override;
    bool               loggedIn() const override;
    void               login(const UserAccount& account, bool persist) override;
    void               restoreSession(const std::string& password) override;
    void               logout() override;
    void               createUser(const UserAccount& account) override;
    bool               canRestoreSession() const override;

private:
    /**
     * @brief Handle login operation completion
     * @param status - Completion status
     * @param session - New user session
     * @param password - User password
     * @param persist - True to persist authentication
     */
    void handleLoggedIn(ApiCallStatus status, const UserSession& session, const std::string& password, bool persist);

    /**
     * @brief Handle logout operation completion
     * @param status - Completion status
     */
    void handleLoggedOut(ApiCallStatus status);

    /**
     * @brief Handle createUser operation completion
     * @param status - Completion status
     */
    void handleUserCreated(ApiCallStatus status);

    /**
     * @brief Handle checkToken operation completion
     * @param status - Completion status
     * @param session - Candidate session to be validated
     * @param password - Password used to decrypt the key storage. If this fails, the authentication will be aborted.
     */
    void handleCheckToken(ApiCallStatus status, const UserSession& session, const std::string& password);

    /**
     * @brief Check if the public and private user keys for the current user exist and update the public key stored on
     * the server if necessary.
     */
    void checkUserKeys();

private:
    /**
     * @brief Current user session. Valid only when logged in.
     */
    UserSession m_session;

    /**
     * @brief Api caller for user operations
     */
    std::unique_ptr<UserApiCaller> m_apiCaller;

    /**
     * @brief Settings manager
     */
    std::shared_ptr<SettingsManager> m_settingsManager;

    /**
     * @brief RSA utilities instance
     */
    std::unique_ptr<Rsa> m_rsa;

    /**
     * @brief Key manager instance
     */
    std::shared_ptr<KeyManager> m_keyManager;

    /**
     * @brief Executor used for generating key pairs asynchronously
     */
    Executor m_rsaExecutor;
};

#endif  // USERSERVICEIMPL_H
