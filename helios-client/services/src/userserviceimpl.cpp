#include <QVariant>
#include <QString>
#include <algorithm>

#include "userserviceimpl.h"
#include "userapicaller.h"
#include "settingsmanager.h"
#include "asyncnotifier.h"
#include "userservicelistener.h"
#include "useraccount.h"

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

const UserSession& UserServiceImpl::session() const
{
    return m_session;
}

bool UserServiceImpl::loggedIn() const
{
    return m_session.valid();
}

void UserServiceImpl::login(const UserAccount& account, bool persist)
{
    if (!m_enabled || !account.valid())
    {
        return;
    }

    m_apiCaller->login(account.username(), account.password(),
                       [this, &account, &persist](ApiCallStatus status, const std::string& authToken) {
                           if (!m_enabled)
                           {
                               return;
                           }
                           handleLoggedIn(status, UserSession(account.username(), authToken), persist);
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
        m_session = UserSession(username.toString().toStdString(), token.toString().toStdString());
    }

    if (m_session.valid())
    {
        for (const auto& listener : m_listeners)
        {
            m_asyncNotifier->postNotification(&UserServiceListener::loggedIn, listener.get(), true, std::string());
        }
    }
}

void UserServiceImpl::logout()
{
    if (!m_enabled || !m_session.valid())
    {
        return;
    }

    m_apiCaller->logout(m_session.authToken(), [this](ApiCallStatus status) {
        if (!m_enabled)
        {
            return;
        }
        handleLoggedOut(status);
    });

    m_settingsManager->reset(SettingsKeys::kUsername);
    m_settingsManager->reset(SettingsKeys::kAuthToken);
    m_session.clear();
}

void UserServiceImpl::createUser(const UserAccount& account)
{
    if (!m_enabled || !account.valid())
    {
        return;
    }

    m_apiCaller->registerUser(account.username(), account.password(), [this](ApiCallStatus status) {
        if (!m_enabled)
        {
            return;
        }
        handleUserCreated(status);
    });
}

void UserServiceImpl::handleLoggedIn(ApiCallStatus status, const UserSession& session, bool persist)
{
    bool        success = false;
    std::string errorString;
    switch (status)
    {
        case ApiCallStatus::SUCCESS:
        {
            m_session = session;
            success   = true;
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
        m_settingsManager->set(SettingsKeys::kUsername, QString::fromStdString(session.username()));
        m_settingsManager->set(SettingsKeys::kAuthToken, QString::fromStdString(session.authToken()));
        m_settingsManager->save();
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
