#include <QDebug>

#include "apicallvisitor.h"
#include "httpstatus.h"

template <typename Call>
void ApiCallVisitor::handlePost(Call& call) const
{
    auto requestManager = m_requestManager.lock();
    if (!requestManager)
    {
        qFatal("HttpRequestManager is not available");
    }
    requestManager->post(
        call.request(), [&call](HttpStatus status, const std::vector<uint8_t>& reply) { call.receive(status, reply); });
}

template <typename Call>
void ApiCallVisitor::handleGet(Call& call) const
{
    auto requestManager = m_requestManager.lock();
    if (!requestManager)
    {
        qFatal("HttpRequestManager is not available");
    }
    requestManager->get(call.request(),
                        [&call](HttpStatus, const std::vector<uint8_t>& reply) { call.receive(status, reply;) });
}
