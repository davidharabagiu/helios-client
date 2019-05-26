#include "notificationsapicallerimpl.h"
#include "apicallvisitor.h"
#include "notificationscall.h"

NotificationsApiCallerImpl::NotificationsApiCallerImpl(const std::shared_ptr<ApiCallVisitor>& visitor)
    : m_visitor(visitor)
{
}

void NotificationsApiCallerImpl::notifications(const std::string&                         authToken,
                                               const ApiCallbacks::NotificationsCallback& callback) const
{
    auto call = new NotificationsCall(authToken, callback);
    call->send(m_visitor);
}
