#include <QDebug>

#include "userapicallerimpl.h"
#include "httprequestmanager.h"
#include "single.h"
#include "dependencyinjector.h"
#include "apicallvisitor.h"
#include "registerusercall.h"
#include "logincall.h"
#include "logoutcall.h"

UserApiCallerImpl::UserApiCallerImpl()
{
    auto requestManager = Single<DependencyInjector>::instance().getInstance<HttpRequestManager>();
    if (!requestManager)
    {
        qFatal("HttpRequestManager instance not available");
    }

    m_visitor = std::make_shared<ApiCallVisitor>(requestManager);
}

void UserApiCallerImpl::registerUser(const std::string& username, const std::string& password,
                                     const ApiCallbacks::RegisterUserCallback& callback) const
{
    RegisterUserCall call(username, password, callback);
    call.send(m_visitor);
}

void UserApiCallerImpl::login(const std::string& username, const std::string& password,
                              const ApiCallbacks::LoginCallback& callback) const
{
    LoginCall call(username, password, callback);
    call.send(m_visitor);
}

void UserApiCallerImpl::logout(const std::string& token, const ApiCallbacks::LogoutCallback& callback) const
{
    LogoutCall call(token, callback);
    call.send(m_visitor);
}
