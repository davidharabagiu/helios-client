#ifndef SERVICEINTERFACE_H
#define SERVICEINTERFACE_H

/**
 * @class ServiceInterface
 * @brief Defines a common interface for all services
 */
class ServiceInterface
{
public:
    virtual ~ServiceInterface() = default;

    /**
     * @brief Enables the service. After a call to this method, operations will be enabled and clients will be
     * notified with changes.
     */
    virtual void start() = 0;

    /**
     * @brief Disables the service. All operations will be disabled and clients will not be notified anymore.
     */
    virtual void stop() = 0;

    /**
     * @brief Returns true if the service is enabled.
     */
    virtual bool enabled() const = 0;
};

#endif  // SERVICEINTERFACE_H
