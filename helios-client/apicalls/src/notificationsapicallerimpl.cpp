#include "notificationsapicallerimpl.h"
#include "apicallvisitor.h"
#include "notificationscall.h"
#include "dismissnotificationcall.h"
#include "dismissallnotificationscall.h"
#include "httprequestmanager.h"

NotificationsApiCallerImpl::NotificationsApiCallerImpl(const std::shared_ptr<HttpRequestManager>& requestManager)
{
    m_visitor.reset(new ApiCallVisitor(requestManager));
}

void NotificationsApiCallerImpl::notifications(const std::string&                         authToken,
                                               const ApiCallbacks::NotificationsCallback& callback) const
{
    auto call = new NotificationsCall(authToken, callback);
    call->send(m_visitor);
}

void NotificationsApiCallerImpl::dismissNotification(const std::string& authToken, const std::string& notificationId,
                                                     const ApiCallbacks::DismissNotificationCallback& callback) const
{
    auto call = new DismissNotificationCall(authToken, notificationId, callback);
    call->send(m_visitor);
}

void NotificationsApiCallerImpl::dismissAllNotifications(
    const std::string& authToken, const ApiCallbacks::DismissAllNotificationsCallback& callback) const
{
    auto call = new DismissAllNotificationsCall(authToken, callback);
    call->send(m_visitor);
}
