#include <QDebug>

#include "apicallvisitor.h"
#include "registerusercall.h"
#include "logincall.h"
#include "logoutcall.h"
#include "createdirectorycall.h"
#include "beginuploadcall.h"
#include "uploadcall.h"
#include "begindownloadcall.h"
#include "downloadcall.h"
#include "endtransfercall.h"
#include "getfilesizecall.h"
#include "listcall.h"
#include "httprequestmanager.h"
#include "httpstatus.h"

ApiCallVisitor::ApiCallVisitor(const std::weak_ptr<HttpRequestManager>& requestManager)
    : m_requestManager(requestManager)
{
}

void ApiCallVisitor::visit(RegisterUserCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(LoginCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(LogoutCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(CreateDirectoryCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(BeginUploadCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(UploadCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(BeginDownloadCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(DownloadCall* call) const
{
    handleGet(call);
}

void ApiCallVisitor::visit(EndTransferCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(GetFileSizeCall* call) const
{
    handleGet(call);
}

void ApiCallVisitor::visit(ListCall* call) const
{
    handleGet(call);
}

void ApiCallVisitor::visit(RemoveCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(MoveCall* call) const
{
    handlePost(call);
}

void ApiCallVisitor::visit(CheckTokenCall* call) const
{
    handleGet(call);
}
