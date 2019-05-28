#include "notificationserviceimpl.h"
#include "config.h"
#include "configkeys.h"
#include "typeconversions.h"
#include "notificationsapicaller.h"

NotificationServiceImpl::NotificationServiceImpl(std::unique_ptr<NotificationsApiCaller> api,
                                                 std::shared_ptr<Config>                 config)
    : m_api(std::move(api))
{
    m_refreshTimerInterval =
        safe_integral_cast<uint32_t>(qvariant_cast<uint>(config->get(ConfigKeys::kNotificationsRefreshInterval)));
}

void NotificationServiceImpl::setSession(const UserSession& session)
{
    removeSession();
    AuthenticatedService::setSession(session);
    refresh();
    m_refreshTimer.start(m_refreshTimerInterval, [this] { refresh(); }, false);
}

void NotificationServiceImpl::removeSession()
{
    m_refreshTimer.stop();
    std::lock_guard<std::mutex> lock(m_mutex);
    AuthenticatedService::removeSession();
    Observable::notifyAll(&NotificationServiceListener::notificationListUpdated);
}

void NotificationServiceImpl::refresh()
{
    if (!enabled())
    {
        return;
    }

    auto authToken = m_session.authToken();
    m_api->notifications(
        authToken,
        [this, authToken](ApiCallStatus                                                              status,
                          const std::vector<std::tuple<std::string, std::string, NotificationType>>& result) {
            if (status == ApiCallStatus::SUCCESS && m_session.authToken() == authToken)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_notifications.clear();
                for (const auto& el : result)
                {
                    m_notifications.emplace(std::piecewise_construct, std::forward_as_tuple(std::get<0>(el)),
                                            std::forward_as_tuple(std::get<0>(el), std::get<1>(el),
                                                                  std::get<2>(el) == NotificationType::FILE_SHARE ?
                                                                      Notification::Type::FILE_SHARE :
                                                                      Notification::Type::KEY_SHARE));
                }
                Observable::notifyAll(&NotificationServiceListener::notificationListUpdated);
            }
        });
}

std::vector<Notification> NotificationServiceImpl::notifications() const
{
    std::vector<Notification>   result;
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& el : m_notifications)
    {
        result.push_back(el.second);
    }
    return result;
}

void NotificationServiceImpl::dismissNotification(const std::string& notificationId)
{
    auto authToken = m_session.authToken();
    m_api->dismissNotification(authToken, notificationId, [this, authToken, notificationId](ApiCallStatus status) {
        if (status == ApiCallStatus::SUCCESS && authToken == m_session.authToken())
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto                        it = m_notifications.find(notificationId);
            if (it != m_notifications.end())
            {
                m_notifications.erase(it);
                Observable::notifyAll(&NotificationServiceListener::notificationListUpdated);
            }
        }
    });
}

void NotificationServiceImpl::dismissAllNotifications()
{
    auto authToken = m_session.authToken();
    m_api->dismissAllNotifications(authToken, [this, authToken](ApiCallStatus status) {
        if (status == ApiCallStatus::SUCCESS && authToken == m_session.authToken())
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_notifications.clear();
            Observable::notifyAll(&NotificationServiceListener::notificationListUpdated);
        }
    });
}
