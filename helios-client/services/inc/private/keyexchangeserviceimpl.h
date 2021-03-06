#ifndef KEYEXCHANGESERVICEIMPL_H
#define KEYEXCHANGESERVICEIMPL_H

#include "keyexchangeservice.h"

// Forward declarations
class KeyManager;
class Rsa;
class UserApiCaller;
class FileApiCaller;
class NotificationsApiCaller;

class KeyExchangeServiceImpl : public KeyExchangeService
{
public:
    /**
     * @brief Constructor
     * @param keyManager - Key manager instance
     * @param rsa - RSA encryption utilities instance
     * @param userApi - User API instance
     * @param fileApi - File API instance
     * @param notificationsApi - Notifications API instance
     */
    KeyExchangeServiceImpl(std::shared_ptr<KeyManager> keyManager, std::unique_ptr<Rsa> rsa,
                           std::unique_ptr<UserApiCaller> userApi, std::unique_ptr<FileApiCaller> fileApi,
                           std::unique_ptr<NotificationsApiCaller> notificationsApi);

public:  // from KeyExchangeService
    void sendKey(const std::string& user, const std::string& keyName) override;
    void receiveKey(const std::string& notificationId) override;

private:
    /**
     * @brief Key manager instance
     */
    std::shared_ptr<KeyManager> m_keyManager;

    /**
     * @brief RSA encryption utilities instance
     */
    std::unique_ptr<Rsa> m_rsa;

    /**
     * @brief User API instance
     */
    std::unique_ptr<UserApiCaller> m_userApi;

    /**
     * @brief File API instance
     */
    std::unique_ptr<FileApiCaller> m_fileApi;

    /**
     * @brief File API instance
     */
    std::unique_ptr<NotificationsApiCaller> m_notificationsApi;
};

#endif  // KEYEXCHANGESERVICEIMPL_H
