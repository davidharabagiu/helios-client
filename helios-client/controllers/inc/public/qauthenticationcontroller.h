#ifndef QAUTHENTICATIONCONTROLLER_H
#define QAUTHENTICATIONCONTROLLER_H

#include <memory>
#include <QString>
#include <QObject>

class QAuthenticationControllerImpl;

/**
 * @class QAuthenticationController
 * @brief Controller for authentication operations (login, logout, register)
 */
class QAuthenticationController : public QObject
{
    Q_OBJECT

    /**
     * @property persistLogin
     * @brief Set to true before calling login in order to persist user authentication
     */
    Q_PROPERTY(bool persistLogin WRITE setPersistLogin)

    /**
     * @property loggedIn
     * @brief True when logged in
     */
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)

    /**
     * @property username
     * @brief Name of the current logged in user. Valid only when logged in.
     */
    Q_PROPERTY(QString username READ username NOTIFY usernameChanged)

public:
    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit QAuthenticationController(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~QAuthenticationController();

    /**
     * @brief Restore last saved session if it exists
     */
    Q_INVOKABLE void restoreSession();

    /**
     * @brief Authenticate
     * @param username - User name
     * @param password - User password
     * @return True if params pass validity checks
     */
    Q_INVOKABLE bool login(const QString& username, const QString& password);

    /**
     * @brief Disconnect from the current session
     * @return True if successful
     */
    Q_INVOKABLE bool logout();

    /**
     * @brief Create user account
     * @param username - User name
     * @param password - User password
     * @return True if params pass validity checks
     */
    Q_INVOKABLE bool createUser(const QString& username, const QString& password);

    /**
     * @brief Setter for persistLogin
     * @param newVal - bool
     */
    void setPersistLogin(bool newVal);

    /**
     * @brief Getter for loggedIn
     * @return bool
     */
    bool loggedIn() const;

    /**
     * @brief Getter for username
     * @return QString
     */
    QString username() const;

signals:
    /**
     * @brief SIGNAL emitted when a login operation completed
     * @param success - True if successful
     * @param errorString - Valid when operation failed, details the occured error
     */
    void loginCompleted(bool success, const QString& errorString);

    /**
     * @brief SIGNAL emitted when a login operation completed
     * @param success - True if successful
     * @param errorString - Valid when operation failed, details the occured error
     */
    void logoutCompleted(bool success, const QString& errorString);

    /**
     * @brief SIGNAL emitted when a login operation completed
     * @param success - True if successful
     * @param errorString - Valid when operation failed, details the occured error
     */
    void userCreationCompleted(bool success, const QString& errorString);

    /**
     * @brief SIGNAL emitted when loggedIn changes
     */
    void loggedInChanged();

    /**
     * @brief SIGNAL emitted when username changes
     */
    void usernameChanged();

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QAuthenticationControllerImpl> m_privateImpl;
};

#endif  // QAUTHENTICATIONCONTROLLER_H
