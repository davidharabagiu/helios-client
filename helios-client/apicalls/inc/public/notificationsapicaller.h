#ifndef NOTIFICATIONSAPICALLER_H
#define NOTIFICATIONSAPICALLER_H

#include <string>
#include <functional>

#include "apicalldefs.h"

/**
 * @class NotificationsApiCaller
 * @brief Provides calls for the notifications management API
 */
class NotificationsApiCaller
{
public:
    /**
     * @brief Destructor
     */
    virtual ~NotificationsApiCaller() = default;

    /**
     * @brief Get all the notifications of the current user
     * @param authToken - Authentication token
     * @param callback - NotificationsCallback
     */
    virtual void notifications(const std::string&                         authToken,
                               const ApiCallbacks::NotificationsCallback& callback) const = 0;
};

#endif  // NOTIFICATIONSAPICALLER_H
