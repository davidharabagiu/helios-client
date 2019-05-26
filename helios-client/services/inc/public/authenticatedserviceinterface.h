#ifndef AUTHENTICATEDSERVICEINTERFACE_H
#define AUTHENTICATEDSERVICEINTERFACE_H

#include <string>

/**
 * @class AuthenticatedServiceInterface
 * @brief Abstraction for an authenticated service i.e. a service which needs an authentication token in order to work
 */
class AuthenticatedServiceInterface
{
public:
    /**
     * @brief Destructor
     */
    virtual ~AuthenticatedServiceInterface() = default;

    /**
     * @brief Returns true if the service is enabled (an auth token is present).
     */
    virtual bool enabled() const = 0;

    /**
     * @brief Set the authentication token of the working user. Having a valid auth token is a precondition for all
     * operations to work.
     * @param authToken - Authentication token
     */
    virtual void setAuthToken(const std::string& authToken) = 0;

    /**
     * @brief Removed the current authentication token. This will disable all operations.
     */
    virtual void removeAuthToken() = 0;
};

#endif  // AUTHENTICATEDSERVICEINTERFACE_H
