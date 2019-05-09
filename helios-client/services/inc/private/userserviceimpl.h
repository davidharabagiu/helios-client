#ifndef USERSERVICEIMPL_H
#define USERSERVICEIMPL_H

#include <memory>
#include <string>
#include <vector>

#include "userservice.h"
#include "apicalldefs.h"
#include "usersession.h"

class UserApiCaller;
class SettingsManager;

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
     */
    UserServiceImpl(std::unique_ptr<UserApiCaller>          userApiCaller,
                    const std::shared_ptr<SettingsManager>& settingsManager);

public:  // from UserService
    const UserSession& session() const override;
    bool               loggedIn() const override;
    void               login(const UserAccount& account, bool persist) override;
    void               restoreSession() override;
    void               logout() override;
    void               createUser(const UserAccount& account) override;

private:
    /**
     * @brief Handle login operation completion
     * @param status - Completion status
     * @param session - New user session
     * @param persist - True to persist authentication
     */
    void handleLoggedIn(ApiCallStatus status, const UserSession& session, bool persist);

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
     */
    void handleCheckToken(ApiCallStatus status, const UserSession& session);

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
};

#endif  // USERSERVICEIMPL_H
