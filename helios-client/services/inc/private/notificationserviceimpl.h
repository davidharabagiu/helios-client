#ifndef NOTIFICATIONSERVICEIMPL_H
#define NOTIFICATIONSERVICEIMPL_H

#include <memory>
#include <map>
#include <string>
#include <mutex>

#include "notificationservice.h"
#include "timer.h"

// Forward declarations
class NotificationsApiCaller;
class Config;

/**
 * @class NotificationServiceImpl
 * @brief Implementation of NotificationService
 */
class NotificationServiceImpl : public NotificationService
{
public:
    /**
     * @brief Constructor
     * @param api - Notifications API instance
     * @param config - Config instance
     */
    NotificationServiceImpl(std::unique_ptr<NotificationsApiCaller> api, std::shared_ptr<Config> config);

public:  // from AuthenticatedService
    void setSession(const UserSession& session) override;
    void removeSession() override;

public:  // from NotificationService
    void                      refresh() override;
    std::vector<Notification> notifications() const override;
    void                      dismissNotification(const std::string& notificationId) override;
    void                      dismissAllNotifications() override;

private:
    /**
     * @brief Notifications API instance
     */
    std::unique_ptr<NotificationsApiCaller> m_api;

    /**
     * @brief Notifications refresh timer
     */
    Timer m_refreshTimer;

    /**
     * @brief Notifications refresh timer interval
     */
    uint32_t m_refreshTimerInterval;

    /**
     * @brief Available notifications mapped by their id
     */
    std::map<std::string, Notification> m_notifications;

    /**
     * @brief Notifications mutex
     */
    mutable std::mutex m_mutex;
};

#endif  // NOTIFICATIONSERVICEIMPL_H
