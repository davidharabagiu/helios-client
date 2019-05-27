#ifndef KEYEXCHANGESERVICEIMPL_H
#define KEYEXCHANGESERVICEIMPL_H

#include "keyexchangeservice.h"

// Forward declarations
class KeyManager;
class Rsa;
class UserApiCaller;
class FileApiCaller;

class KeyExchangeServiceImpl : public KeyExchangeService
{
public:
    /**
     * @brief Constructor
     * @param keyManager - Key manager instance
     * @param rsa - RSA encryption utilities instance
     * @param userApi - User API instance
     * @param fileApi - File API instance
     */
    KeyExchangeServiceImpl(std::shared_ptr<KeyManager> keyManager, std::unique_ptr<Rsa> rsa,
                           std::unique_ptr<UserApiCaller> userApi, std::unique_ptr<FileApiCaller> fileApi);

public:  // from AuthenticatedServiceInterface
    bool enabled() const override;
    void setAuthToken(const std::string& authToken) override;
    void removeAuthToken() override;

public:  // from KeyExchangeService
    void sendKey(const std::string& user, const std::string& keyName) override;

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
     * @brief Current user's authentication token
     */
    std::string m_authToken;
};

#endif  // KEYEXCHANGESERVICEIMPL_H
