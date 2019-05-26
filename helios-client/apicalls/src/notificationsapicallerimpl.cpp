#include "notificationsapicallerimpl.h"
#include "apicallvisitor.h"
#include "notificationscall.h"
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
