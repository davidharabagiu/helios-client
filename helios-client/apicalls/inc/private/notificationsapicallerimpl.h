#ifndef NOTIFICATIONAPICALLERIMPL_H
#define NOTIFICATIONAPICALLERIMPL_H

#include "notificationsapicaller.h"

// Forward declarations
class ApiCallVisitor;
class HttpRequestManager;

/**
 * @class NotificationsApiCallerImpl
 * @brief Implementation of NotificationApiCaller
 */
class NotificationsApiCallerImpl : public NotificationsApiCaller
{
public:
    /**
     * @brief Constructor
     * @param requestManager - HTTP request manager instance
     */
    NotificationsApiCallerImpl(const std::shared_ptr<HttpRequestManager>& requestManager);

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
