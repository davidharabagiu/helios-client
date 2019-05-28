#ifndef QUSERSESSION_H
#define QUSERSESSION_H

#include <QMetaType>
#include <QString>
#include <memory>

// Forward declarations
class UserSession;

/**
 * @class QUserSession
 * @brief Qt wrapper over UserSession model class
 */
class QUserSession
{
    Q_GADGET

    /**
     * @property isValid
     * @brief Validity check
     */
    Q_PROPERTY(bool isValid READ isValid)

    /**
     * @property username
     * @brief Name of the authenticated user
     */
    Q_PROPERTY(QString username READ username)

    /**
     * @property authToken
     * @brief Authentication token of the user
     */
    Q_PROPERTY(QString authToken READ authToken)

public:
    /**
     * @bried Constructor
     */
    QUserSession() = default;

    /**
     * @brief Constructor
     * @param data - Internal data pointer
     */
    QUserSession(const std::shared_ptr<const UserSession>& data);

    /**
     * @brief Getter for isValid
     * @return bool
     */
    bool isValid() const;

    /**
     * @brief Getter for username
     * @return QString
     */
    QString username() const;

    /**
     * @brief Getter for authToken
     * @return QString
     */
    QString authToken() const;

    /**
     * @brief Getter for m_data
     * @return std::shared_ptr<const UserSesion>
     */
    std::shared_ptr<const UserSession> data() const;

private:
    /**
     * @brief Internal data pointer
     */
    std::shared_ptr<const UserSession> m_data;
};

#endif  // QUSERSESSION_H
