#include <QDebug>

#include "apicallvisitor.h"
#include "httpstatus.h"
#include "httprequestmanager.h"
#include "logincall.h"
#include "logoutcall.h"
#include "checktokencall.h"
#include "getuserkeycall.h"
#include "setuserkeycall.h"
#include "begindownloadcall.h"
#include "beginuploadcall.h"
#include "createdirectorycall.h"
#include "downloadcall.h"
#include "endtransfercall.h"
#include "getfilesizecall.h"
#include "listcall.h"
#include "movecall.h"
#include "registerusercall.h"
#include "removecall.h"
#include "uploadcall.h"
#include "isdircall.h"
#include "notificationscall.h"
#include "typeutils.h"

template <typename Call>
void ApiCallVisitor::handlePost(Call* call) const
{
    auto requestManager = m_requestManager.lock();
    if (!requestManager)
    {
        qFatal("HttpRequestManager is not available");
    }
    requestManager->post(call->request(), [call](HttpStatus status, const std::vector<uint8_t>& reply, bool success) {
        if (!success)
        {
            qCritical() << "Failed http request from API call of type" << TypeUtils::getTypeName<Call>().c_str();
        }
        call->receive(status, reply);
        delete call;
    });
}

template <typename Call>
void ApiCallVisitor::handleGet(Call* call) const
{
    auto requestManager = m_requestManager.lock();
    if (!requestManager)
    {
        qFatal("HttpRequestManager is not available");
    }
    requestManager->get(call->request(), [call](HttpStatus status, const std::vector<uint8_t>& reply, bool success) {
        if (!success)
        {
            qCritical() << "Failed http request from API call of type" << TypeUtils::getTypeName<Call>().c_str();
        }
        call->receive(status, reply);
        delete call;
    });
}
