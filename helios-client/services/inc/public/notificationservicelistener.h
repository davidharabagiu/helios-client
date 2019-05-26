#ifndef NOTIFICATIONSERVICELISTENER_H
#define NOTIFICATIONSERVICELISTENER_H

/**
 * @class NotificationServiceListener
 * @brief Interface for listening on NotificationService async operations
 */
class NotificationServiceListener
{
public:
    /**
     * @brief Destructor
     */
    virtual ~NotificationServiceListener() = default;

    /**
     * @brief Called when the notification list has been updated
     */
    virtual void notificationListUpdated() = 0;
};

#endif  // NOTIFICATIONSERVICELISTENER_H
