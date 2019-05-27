#ifndef KEYMANAGERLISTENER_H
#define KEYMANAGERLISTENER_H

#include <string>

/**
 * @class KeyManagerListener
 * @brief Listener interface for KeyManager asynchronous operations
 */
class KeyManagerListener
{
public:
    /**
     * @brief Destructor
     */
    ~KeyManagerListener() = default;

    /**
     * @brief An asynchronous operation has failed
     * @param errorString - Error message
     */
    virtual void errorOccured(const std::string& errorString) = 0;
};

#endif  // KEYMANAGERLISTENER_H
