#ifndef KEYEXCHANGESERVICELISTENER_H
#define KEYEXCHANGESERVICELISTENER_H

#include <string>

/**
 * @class KeyExchangeServiceListener
 * @brief Listener interface for KeyExchangeListener asynchronous operations
 */
class KeyExchangeServiceListener
{
public:
    /**
     * @brief Errors that may occur during service operations
     */
    enum class Error
    {
        /**
         * @brief The requested key could not be found
         */
        NO_SUCH_KEY,

        /**
         * @brief The key sender cannot handle transfers at this time
         */
        SENDER_DISABLED_TRANSFERS,

        /**
         * @brief The key recipient doesn't accept any transfers at this time
         */
        RECIPIENT_DISABLED_TRANSFERS,

        /**
         * @brief The specified key recipient doesn't exist
         */
        UNKNOWN_USER,

        /**
         * @brief Some other unhandled error occured
         */
        UNKNWOWN_ERROR
    };

    /**
     * @brief Destructor
     */
    virtual ~KeyExchangeServiceListener() = default;

    /**
     * @brief A key sharing operation completed successfully
     */
    virtual void keySharedSuccessfully() = 0;

    /**
     * @brief A key sharing operation has failed
     * @param error - Error type
     */
    virtual void keyShareError(Error error) = 0;

    /**
     * @brief A key accepting / receiving operation completed successfully
     */
    virtual void keyReceivedSuccessfully() = 0;

    /**
     * @brief A key accepting / receiving operation has failed
     * @param error - Error type
     */
    virtual void keyReceiveError(Error error) = 0;
};

#endif  // KEYEXCHANGESERVICELISTENER_H
