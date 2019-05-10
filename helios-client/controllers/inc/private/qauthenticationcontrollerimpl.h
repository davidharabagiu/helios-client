#ifndef QAUTHENTICATIONCONTROLLERIMPL_H
#define QAUTHENTICATIONCONTROLLERIMPL_H

#include <memory>
#include <QString>

#include "userservicelistener.h"

class QAuthenticationController;
class UserService;

/**
 * @class QAuthenticationControllerImpl
 * @brief Private implementation of QAuthenticationController
 */
class QAuthenticationControllerImpl : public UserServiceListener,
                                      public std::enable_shared_from_this<QAuthenticationControllerImpl>
{
public:
    /**
     * @brief Constructor
     * @param publicImpl - Public implementation
     */
    QAuthenticationControllerImpl(QAuthenticationController* publicImpl);

    /**
     * @brief Register for notifications from internal services
     */
    void registerForNotifications();

    /**
     * @brief Unregister from notifications from internal services
     */
    void unregisterFromNotifications();

public:  // from UserServiceListener
    void loginCompleted(bool success, const std::string& errorString) override;
    void logoutCompleted(bool success, const std::string& errorString) override;
    void userCreationCompleted(bool success, const std::string& errorString) override;

public:  // forwarded from QAuthenticationController
    void    restoreSession();
    bool    login(const QString& username, const QString& password);
    bool    logout();
    bool    createUser(const QString& username, const QString& password);
    void    setPersistLogin(bool newVal);
    bool    loggedIn() const;
    QString username() const;
    QString authenticationToken() const;

private:
    /**
     * @brief Public implementation
     */
    QAuthenticationController* m_publicImpl;

    /**
     * @brief User service
     */
    std::shared_ptr<UserService> m_userService;

    /**
     * @brief Persist login
     */
    bool m_persistLogin;
};

#endif  // QAUTHENTICATIONCONTROLLERIMPL_H
