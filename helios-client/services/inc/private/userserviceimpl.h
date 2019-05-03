#ifndef USERSERVICEIMPL_H
#define USERSERVICEIMPL_H

#include <memory>
#include <string>
#include <vector>

#include "userservice.h"
#include "apicalldefs.h"

class UserApiCaller;
class SettingsManager;
class AsyncNotifier;

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

public:  // from ServiceInterface
    void start() override;
    void stop() override;

public:  // from UserService
    void               registerListener(const std::shared_ptr<UserServiceListener>& listener) override;
    void               unregisterListener(const std::shared_ptr<UserServiceListener>& listener) override;
    const std::string& username() const override;
    const std::string& authenticationToken() const override;
    void               login(const std::string& username, const std::string& password, bool persist) override;
    void               restoreSession() override;
    void               logout() override;
    void               createUser(const std::string& username, const std::string& password) override;

private:
    /**
     * @brief Save session so it can be restored when the application restarts.
     * @param username - User name
     * @param authToken - Authentication token
     */
    void saveSession(const std::string& username, const std::string& authToken);

    /**
     * @brief Handle login operation completion
     * @param status - Completion status
     * @param username - User name
     * @param authToken - Authentication token
     * @param persist - True to persist authentication
     */
    void handleLoggedIn(ApiCallStatus status, const std::string& username, const std::string& authToken, bool persist);

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

private:
    /**
     * @brief True if the service has be started
     */
    bool m_enabled;

    /**
     * @brief Name of the currently logged in user
     */
    std::string m_username;

    /**
     * @brief Authentication token of the currenly logged in user
     */
    std::string m_authToken;

    /**
     * @brief Operations listeners
     */
    std::vector<std::shared_ptr<UserServiceListener>> m_listeners;

    /**
     * @brief Api caller for user operations
     */
    std::unique_ptr<UserApiCaller> m_apiCaller;

    /**
     * @brief Settings manager
     */
    std::shared_ptr<SettingsManager> m_settingsManager;

    /**
     * @brief Used for asynchronous notification of clients
     */
    std::unique_ptr<AsyncNotifier> m_asyncNotifier;
};

#endif  // USERSERVICEIMPL_H
