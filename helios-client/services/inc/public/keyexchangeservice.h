#ifndef KEYEXCHANGESERVICE_H
#define KEYEXCHANGESERVICE_H

#include "authenticatedserviceinterface.h"
#include "observable.h"
#include "keyexchangeservicelistener.h"

class KeyExchangeService : public AuthenticatedServiceInterface,
                           public Observable<KeyExchangeServiceListener, ObservableNotifyMode::ASYNC>
{
public:
    /**
     * @brief Send a key to another user
     * @param user - Name of the destination of the key
     * @param keyName - Identity name of the key
     */
    virtual void sendKey(const std::string& user, const std::string& keyName) = 0;
};

#endif  // KEYEXCHANGESERVICE_H
