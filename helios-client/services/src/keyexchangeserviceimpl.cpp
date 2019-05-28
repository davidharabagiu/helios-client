#include <QFile>

#include "keyexchangeserviceimpl.h"
#include "keymanager.h"
#include "rsa.h"
#include "userapicaller.h"
#include "fileapicaller.h"
#include "paths.h"
#include "typeconversions.h"

KeyExchangeServiceImpl::KeyExchangeServiceImpl(std::shared_ptr<KeyManager> keyManager, std::unique_ptr<Rsa> rsa,
                                               std::unique_ptr<UserApiCaller> userApi,
                                               std::unique_ptr<FileApiCaller> fileApi)
    : m_keyManager(keyManager)
    , m_rsa(std::move(rsa))
    , m_userApi(std::move(userApi))
    , m_fileApi(std::move(fileApi))
{
}

void KeyExchangeServiceImpl::sendKey(const std::string& user, const std::string& keyName)
{
    if (!enabled())
    {
        return;
    }

    auto fileKey = m_keyManager->getKey(keyName);
    if (fileKey.empty())
    {
        Observable::notifyAll(&KeyExchangeServiceListener::keyShareError,
                              KeyExchangeServiceListener::Error::NO_SUCH_KEY);
        return;
    }

    auto authToken = m_session.authToken();

    m_userApi->getUserKey(
        authToken, user, [this, authToken, fileKey, user, keyName](ApiCallStatus status, const std::string& userKey) {
            if (status == ApiCallStatus::SUCCESS)
            {
                QFile userKeyFile(QString::fromStdString(Paths::kTemporaryPublicKeyFile));
                userKeyFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
                userKeyFile.write(QByteArray::fromStdString(userKey));
                userKeyFile.close();

                auto encryptedFileKey = m_rsa->encrypt(Paths::kTemporaryPublicKeyFile, fileKey);
                userKeyFile.remove();

                m_fileApi->shareKey(authToken, user, keyName, safe_integral_cast<uint16_t>(fileKey.size()),
                                    encryptedFileKey, [this](ApiCallStatus status) {
                                        if (status == ApiCallStatus::SUCCESS)
                                        {
                                            Observable::notifyAll(&KeyExchangeServiceListener::keySharedSuccessfully);
                                        }
                                        else
                                        {
                                            Observable::notifyAll(&KeyExchangeServiceListener::keyShareError,
                                                                  KeyExchangeServiceListener::Error::UNKNWOWN_ERROR);
                                        }
                                    });
            }
            else if (status == ApiCallStatus::NO_KEY_SPECIFIED)
            {
                Observable::notifyAll(&KeyExchangeServiceListener::keyShareError,
                                      KeyExchangeServiceListener::Error::RECIPIENT_DISABLED_TRANSFERS);
            }
            else if (status == ApiCallStatus::INVALID_USERNAME)
            {
                Observable::notifyAll(&KeyExchangeServiceListener::keyShareError,
                                      KeyExchangeServiceListener::Error::UNKNOWN_USER);
            }
            else
            {
                Observable::notifyAll(&KeyExchangeServiceListener::keyShareError,
                                      KeyExchangeServiceListener::Error::UNKNWOWN_ERROR);
            }
        });
}

void KeyExchangeServiceImpl::receiveKey(const std::string& /*notificationId*/)
{
    if (!enabled())
    {
        return;
    }

    // To be implemented
}
