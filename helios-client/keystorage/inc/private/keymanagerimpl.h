#ifndef KEYMANAGERIMPL_H
#define KEYMANAGERIMPL_H

#include <cstdint>
#include <memory>
#include <QMap>
#include <QByteArray>
#include <QString>
#include <QFile>

#include "keymanager.h"

// Forward declarations
class RandomFactory;
class Random;
class CipherFactory;
class Cipher;

/**
 * @class KeyManagerImpl
 * @brief Private implementation of KeyManager
 */
class KeyManagerImpl : public KeyManager
{
public:
    /**
     * @brief Constructor
     * @param randomFactory - RNG factory instance
     * @param cipherFactory - Cipher factory instance
     */
    KeyManagerImpl(std::unique_ptr<RandomFactory> randomFactory, std::unique_ptr<CipherFactory> cipherFactory);

public:  // from KeyManager
    void                     loadKeys(const std::string& username, const std::string& password) override;
    void                     unloadKeys() override;
    std::vector<std::string> listKeys(uint16_t length) const override;
    bool                     createKey(const std::string& name, uint16_t length) override;
    void                     addKey(const std::string& name, const std::vector<uint8_t>& content) override;
    std::vector<uint8_t>     getKey(const std::string& name) const override;
    bool                     removeKey(const std::string& name) override;
    void                     removeAllKeys() override;

private:
    /**
     * @brief Persist all keys in encrypted storage
     */
    void persistKeys() const;

private:
    /**
     * @brief Cached keys mapped by name
     */
    QMap<QString, QByteArray> m_keys;

    /**
     * @brief RNG instance
     */
    std::shared_ptr<Random> m_rng;

    /**
     * @brief Cipher instance
     */
    std::shared_ptr<Cipher> m_cipher;

    /**
     * @brief Key storage path
     */
    std::string m_storagePath;

    /**
     * @brief Key used for storage encryption and decryption
     */
    std::vector<uint8_t> m_encryptionKey;
};

#endif  // KEYMANAGERIMPL_H
