#ifndef QNOTIFICATION_H
#define QNOTIFICATION_H

#include <QMetaType>
#include <QString>
#include <memory>

#include "notification.h"

/**
 * @class QNotification
 * @brief Qt wrapper over Notification model
 */
class QNotification
{
    Q_GADGET

    /**
     * @property isValid
     * @brief Validity check
     */
    Q_PROPERTY(bool isValid READ isValid)

    /**
     * @property id
     * @brief Notification id
     */
    Q_PROPERTY(QString id READ id)

    /**
     * @property text
     * @brief Notification content
     */
    Q_PROPERTY(QString text READ text)

    /**
     * @property type
     * @brief Notification type
     */
    Q_PROPERTY(QNotificationType type READ type)

public:
    /**
     * @brief Notification type enumeration
     */
    enum QNotificationType
    {
        /**
         * @brief Notifications of this type inform the user that a file was shared with him
         */
        FILE_SHARE = static_cast<int>(Notification::Type::FILE_SHARE),

        /**
         * @brief Notifications of this type inform the user that a file key was shared with him
         */
        KEY_SHARE = static_cast<int>(Notification::Type::KEY_SHARE)
    };
    Q_ENUM(QNotificationType);

    /**
     * @brief Constructs an invalid QNotification
     */
    QNotification() = default;

    /**
     * @brief Constructor
     * @param data - Internal data
     */
    QNotification(const std::shared_ptr<const Notification>& data);

    /**
     * @brief Getter for isValid
     * @return bool
     */
    bool isValid() const;

    /**
     * @brief Getter for id
     * @return QString
     */
    QString id() const;

    /**
     * @brief Getter for text
     * @return QString
     */
    QString text() const;

    /**
     * @brief Getter for type
     * @return QNotification type
     */
    QNotificationType type() const;

    /**
     * @brief Getter for m_data
     * @return std::shared_ptr<const Notification>
     */
    std::shared_ptr<const Notification> data() const;

private:
    /**
     * @brief Internal data
     */
    std::shared_ptr<const Notification> m_data;
};

#endif  // QNOTIFICATION_H
