#ifndef USERSERVICELISTENER_H
#define USERSERVICELISTENER_H

#include <string>

/**
 * @class UserServiceListener
 * @brief Interface for listening on UserService async operations
 */
class UserServiceListener
{
public:
    /**
     * @brief Destructor
     */
    virtual ~UserServiceListener() = default;

    /**
     * @brief Called when a user login operation completed
     * @param success - Specified whether or not the operation completed successfully
     * @param errorString - Valid only when success is false. It details the occured problem.
     */
    virtual void loginCompleted(bool success, const std::string& errorString) = 0;

    /**
     * @brief Called when a user logout operation completed
     * @param success - Specified whether or not the operation completed successfully
     * @param errorString - Valid only when success is false. It details the occured problem.
     */
    virtual void logoutCompleted(bool success, const std::string& errorString) = 0;

    /**
     * @brief Called when a user registration operation completed
     * @param success - Specified whether or not the operation completed successfully
     * @param errorString - Valid only when success is false. It details the occured problem.
     */
    virtual void userCreationCompleted(bool success, const std::string& errorString) = 0;
};

#endif  // USERSERVICELISTENER_H
