#include <QVariant>
#include <QString>
#include <algorithm>

#include "userserviceimpl.h"
#include "userapicaller.h"
#include "settingsmanager.h"
#include "asyncnotifier.h"
#include "userservicelistener.h"

namespace SettingsKeys
{
/**
 * @brief Username persistence setting key
 */
const std::string kUsername = "username";
/**
 * @brief Authentication token pesistence setting key
 */
const std::string kAuthToken = "token";
}  // namespace SettingsKeys

UserServiceImpl::UserServiceImpl(std::unique_ptr<UserApiCaller>          userApiCaller,
                                 const std::shared_ptr<SettingsManager>& settingsManager)
    : m_apiCaller(std::move(userApiCaller))
    , m_settingsManager(settingsManager)
    , m_asyncNotifier(new AsyncNotifier())
{
}

void UserServiceImpl::start()
{
    m_enabled = true;
}

void UserServiceImpl::stop()
{
    m_enabled = false;
    m_asyncNotifier->cancelPendingNotifications();
}

void UserServiceImpl::registerListener(const std::shared_ptr<UserServiceListener>& listener)
{
    m_listeners.push_back(listener);
}

void UserServiceImpl::unregisterListener(const std::shared_ptr<UserServiceListener>& listener)
{
    m_listeners.erase(std::find(m_listeners.begin(), m_listeners.end(), listener));
}

const std::string& UserServiceImpl::username() const
{
    return m_username;
}

const std::string& UserServiceImpl::authenticationToken() const
{
    return m_authToken;
}

void UserServiceImpl::login(const std::string& username, const std::string& password, bool persist)
{
    if (!m_enabled)
    {
        return;
    }

    m_apiCaller->login(username, password,
                       [this, &username, &persist](ApiCallStatus status, const std::string& authToken) {
                           if (!m_enabled)
                           {
                               return;
                           }
                           handleLoggedIn(status, username, authToken, persist);
                       });
}

void UserServiceImpl::restoreSession()
{
    if (!m_enabled)
    {
        return;
    }

    auto username = m_settingsManager->get(SettingsKeys::kUsername);
    auto token    = m_settingsManager->get(SettingsKeys::kAuthToken);

    if (username.isValid() && token.isValid())
    {
        m_username  = username.toString().toStdString();
        m_authToken = token.toString().toStdString();
    }

    for (const auto& listener : m_listeners)
    {
        m_asyncNotifier->postNotification(&UserServiceListener::loggedIn, listener.get(), true, std::string());
    }
}

void UserServiceImpl::logout()
{
    if (!m_enabled)
    {
        return;
    }

    m_apiCaller->logout(m_authToken, [this](ApiCallStatus status) {
        if (!m_enabled)
        {
            return;
        }
        handleLoggedOut(status);
    });

    m_settingsManager->reset(SettingsKeys::kUsername);
    m_settingsManager->reset(SettingsKeys::kAuthToken);
    m_username.clear();
    m_authToken.clear();
}

void UserServiceImpl::createUser(const std::string& username, const std::string& password)
{
    if (!m_enabled)
    {
        return;
    }

    m_apiCaller->registerUser(username, password, [this](ApiCallStatus status) {
        if (!m_enabled)
        {
            return;
        }
        handleUserCreated(status);
    });

    m_settingsManager->reset(SettingsKeys::kUsername);
    m_settingsManager->reset(SettingsKeys::kAuthToken);
    m_username.clear();
    m_authToken.clear();
}

void UserServiceImpl::saveSession(const std::string& username, const std::string& authToken)
{
    m_settingsManager->set(SettingsKeys::kUsername, QString::fromStdString(username));
    m_settingsManager->set(SettingsKeys::kAuthToken, QString::fromStdString(authToken));
    m_settingsManager->save();
}

void UserServiceImpl::handleLoggedIn(ApiCallStatus status, const std::string& username, const std::string& authToken,
                                     bool persist)
{
    bool        success = false;
    std::string errorString;
    switch (status)
    {
        case ApiCallStatus::SUCCESS:
        {
            m_username  = username;
            m_authToken = authToken;
            success     = true;
            break;
        }
        case ApiCallStatus::INVALID_USERNAME:
        {
            errorString = "Invalid username";
            break;
        }
        case ApiCallStatus::INVALID_PASSWORD:
        {
            errorString = "Invalid password";
            break;
        }
        default:
        {
            errorString = "Unknown error";
            break;
        }
    }

    for (const auto& listener : m_listeners)
    {
        m_asyncNotifier->postNotification(&UserServiceListener::loggedIn, listener.get(), success, errorString);
    }

    if (persist)
    {
        saveSession(username, authToken);
    }
}

void UserServiceImpl::handleLoggedOut(ApiCallStatus status)
{
    bool        success = false;
    std::string errorString;
    if (status == ApiCallStatus::SUCCESS)
    {
        success = true;
    }
    else
    {
        errorString = (status == ApiCallStatus::UNAUTHORIZED) ? "Invalid token" : "Unknown error";
    }

    for (const auto& listener : m_listeners)
    {
        m_asyncNotifier->postNotification(&UserServiceListener::loggedOut, listener.get(), success, errorString);
    }
}

void UserServiceImpl::handleUserCreated(ApiCallStatus status)
{
    bool        success = false;
    std::string errorString;
    switch (status)
    {
        case ApiCallStatus::SUCCESS:
        {
            success = true;
            break;
        }
        case ApiCallStatus::USERNAME_ALREADY_EXISTS:
        {
            errorString = "This username already exists";
            break;
        }
        case ApiCallStatus::INVALID_USERNAME:
        {
            errorString = "Invalid username";
            break;
        }
        case ApiCallStatus::INVALID_PASSWORD:
        {
            errorString = "Invalid password";
            break;
        }
        default:
        {
            errorString = "Unknown error";
            break;
        }
    }

    for (const auto& listener : m_listeners)
    {
        m_asyncNotifier->postNotification(&UserServiceListener::loggedOut, listener.get(), success, errorString);
    }
}
