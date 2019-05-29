#ifndef QAUTHENTICATIONCONTROLLER_H
#define QAUTHENTICATIONCONTROLLER_H

#include <memory>
#include <QString>
#include <QObject>

#include "qusersession.h"

// Forward declarations
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
     * @property session
     * @brief Current session. Valid only when logged in.
     */
    Q_PROPERTY(QUserSession session READ session NOTIFY sessionChanged)

    /**
     * @property hasPersistedLogin
     * @brief Use this to check if the user has any persisted login (stored token) before performing the restoreSession
     * operation.
     */
    Q_PROPERTY(bool hasPersistedLogin READ hasPersistedLogin)

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
     * @param password - Password used for key storage decryption
     */
    Q_INVOKABLE void restoreSession(const QString& password);

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
     * @brief Getter for session
     * @return QUserSession
     */
    QUserSession session() const;

    /**
     * @brief Getter for hasPersistedLogin
     * @return bool
     */
    bool hasPersistedLogin() const;

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
     * @brief SIGNAL emitted when session changes
     */
    void sessionChanged();

    /**
     * @brief SIGNAL emitted when the key storage decryption phase in the restore session operation has failed (the
     * password is wrong).
     */
    void keyStorageDecryptionFailed();

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QAuthenticationControllerImpl> m_privateImpl;
};

#endif  // QAUTHENTICATIONCONTROLLER_H
