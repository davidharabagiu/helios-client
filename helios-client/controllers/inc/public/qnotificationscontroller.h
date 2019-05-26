#ifndef QNOTIFICATIONSCONTROLLER_H
#define QNOTIFICATIONSCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <memory>

// Forward declarations
class QNotificationsControllerImpl;

/**
 * @class QNotificationsController
 * @brief Controller for notification operations (query, dismiss)
 */
class QNotificationsController : public QObject
{
    Q_OBJECT

    /**
     * @property authenticationToken
     * @brief Authentication token of the current user session used to access the user's remote files
     */
    Q_PROPERTY(QString authenticationToken WRITE setAuthenticationToken RESET resetAuthenticationToken)

    /**
     * @property notifications
     * @brief List of current notifications. QVariantList of QNotification
     */
    Q_PROPERTY(QVariantList notifications READ notifications NOTIFY notificationsChanged)

public:
    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit QNotificationsController(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~QNotificationsController();

    /**
     * @brief Setter for authenticationToken
     * @param newValue - QString
     */
    void setAuthenticationToken(const QString& newValue);

    /**
     * @brief Remove the current authentication token
     */
    void resetAuthenticationToken();

    /**
     * @brief Getter for notifications
     * @return QVariantList of QNotifications
     */
    QVariantList notifications() const;

signals:
    /**
     * @brief SIGNAL emitted when notifications changes
     */
    void notificationsChanged();

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QNotificationsControllerImpl> m_privateImpl;
};

#endif  // QNOTIFICATIONSCONTROLLER_H
