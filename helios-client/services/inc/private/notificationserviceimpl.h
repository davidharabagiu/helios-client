#ifndef NOTIFICATIONSERVICEIMPL_H
#define NOTIFICATIONSERVICEIMPL_H

#include <memory>
#include <map>
#include <string>

#include "notificationservice.h"

// Forward declarations
class NotificationsApiCaller;

/**
 * @class NotificationServiceImpl
 * @brief Implementation of NotificationService
 */
class NotificationServiceImpl : public NotificationService
{
public:
    NotificationServiceImpl(std::unique_ptr<NotificationsApiCaller> api);

public:  // from AuthenticatedServiceInterface
    bool enabled() const override;
    void setAuthToken(const std::string& authToken) override;
    void removeAuthToken() override;

public:  // from NotificationService
    void                      refresh() override;
    std::vector<Notification> notifications() const override;

private:
    /**
     * @brief Notifications API instance
     */
    std::unique_ptr<NotificationsApiCaller> m_api;

    /**
     * @brief Authentication token
     */
    std::string m_authToken;

    /**
     * @brief Available notifications mapped by their id
     */
    std::map<std::string, Notification> m_notifications;
};

#endif  // NOTIFICATIONSERVICEIMPL_H
