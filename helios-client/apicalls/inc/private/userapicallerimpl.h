#ifndef USERAPICALLERIMPL_H
#define USERAPICALLERIMPL_H

#include <memory>

#include "userapicaller.h"

class ApiCallVisitor;
class HttpRequestManager;

/**
 * @class UserApiCallerImpl
 * @brief Private implementation of UserApiCaller
 */
class UserApiCallerImpl : public UserApiCaller
{
public:
    /**
     * @brief Constructor
     * @param requestManager - Http request manager instance
     */
    UserApiCallerImpl(const std::shared_ptr<HttpRequestManager>& requestManager);

public:  // from UserApiCaller
    void registerUser(const std::string& username, const std::string& password,
                      const ApiCallbacks::RegisterUserCallback& callback) const override;
    void login(const std::string& username, const std::string& password,
               const ApiCallbacks::LoginCallback& callback) const override;
    void logout(const std::string& token, const ApiCallbacks::LogoutCallback& callback) const override;
    void checkToken(const std::string& username, const std::string& token,
                    const ApiCallbacks::CheckTokenCallback& callback) const override;
    void getUserKey(const std::string& token, const std::string& username,
                    const ApiCallbacks::GetUserKeyCallback& callback) const override;

private:
    /**
     * @brief API call visitor
     */
    std::shared_ptr<ApiCallVisitor> m_visitor;
};

#endif  // USERAPICALLERIMPL_H
