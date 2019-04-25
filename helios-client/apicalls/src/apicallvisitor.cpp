#include <QDebug>

#include "apicallvisitor.h"
#include "registerusercall.h"
#include "httprequestmanager.h"

ApiCallVisitor::ApiCallVisitor(const std::weak_ptr<HttpRequestManager>& requestManager)
    : m_requestManager(requestManager)
{
}

void ApiCallVisitor::visit(RegisterUserCall& call) const
{
    auto requestManager = m_requestManager.lock();
    if (!requestManager)
    {
        qFatal("HttpRequestManager is not available");
    }
    requestManager->post(
        call.request(), [&call](HttpStatus status, const std::vector<uint8_t>& reply) { call.receive(status, reply); });
}
