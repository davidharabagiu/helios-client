#ifndef NOTIFICATIONAPICALLERIMPL_H
#define NOTIFICATIONAPICALLERIMPL_H

#include "notificationsapicaller.h"

// Forward declarations
class ApiCallVisitor;

/**
 * @class NotificationsApiCallerImpl
 * @brief Implementation of NotificationApiCaller
 */
class NotificationsApiCallerImpl : public NotificationsApiCaller
{
public:
    /**
     * @brief Constructor
     * @param visitor - API call visitor instance
     */
    NotificationsApiCallerImpl(const std::shared_ptr<ApiCallVisitor>& visitor);

public:  // from NotificationsApiCaller
    void notifications(const std::string&                         authToken,
                       const ApiCallbacks::NotificationsCallback& callback) const override;

private:
    /**
     * @brief API call visitor
     */
    std::shared_ptr<ApiCallVisitor> m_visitor;
};

#endif  // NOTIFICATIONSAPICALLERIMPL_H
