#include <QVariant>

#include "userserviceimpl.h"
#include "userapicaller.h"
#include "settingsmanager.h"

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
{
}

void UserServiceImpl::start()
{
    m_enabled = true;
}

void UserServiceImpl::stop()
{
    m_enabled = false;
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
}
