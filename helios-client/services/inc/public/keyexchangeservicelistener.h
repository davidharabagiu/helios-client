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
    ~KeyExchangeServiceListener() = default;

    /**
     * @brief A key sharing operation completed successfully
     */
    virtual void keySharedSuccessfully() = 0;

    /**
     * @brief An asynchronous operation has failed
     * @param errorString - Error message
     */
    virtual void errorOccured(Error error) = 0;
};

#endif  // KEYEXCHANGESERVICELISTENER_H
