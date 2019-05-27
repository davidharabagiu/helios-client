#ifndef QKEYSTORAGECONTROLLERIMPL_H
#define QKEYSTORAGECONTROLLERIMPL_H

#include <QStringList>
#include <QString>
#include <memory>
#include <cstdint>

// Forward declarations
class KeyManager;
class KeyExchangeService;
class QKeyStorageController;

/**
 * @class QKeyStorageControllerImpl
 * @brief Private implementation of QKeyStorageController
 */
class QKeyStorageControllerImpl
{
public:
    /**
     * @brief Specify a key size in bit count
     */
    enum class KeySize
    {
        _128,
        _192,
        _256
    };

    /**
     * @brief Constructor
     * @param publicImpl - Public implementation
     */
    QKeyStorageControllerImpl(QKeyStorageController* publicImpl);

public:  // Forwarded from QKeyStorageController
    QStringList keys(KeySize keySize) const;
    bool        createKey(const QString& name, KeySize size);
    bool        removeKey(const QString& name);
    void        removeAllKeys();
    void        sendKey(const QString& username, const QString& keyName);

private:
    static uint16_t keySizeToByteLength(KeySize keySize);

private:
    /**
     * @brief Public implementation
     */
    QKeyStorageController* m_publicImpl;

    /**
     * @brief Key manager instance
     */
    std::shared_ptr<KeyManager> m_keyManager;

    /**
     * @brief Key exchange service instance
     */
    std::shared_ptr<KeyExchangeService> m_keyExchangeService;
};

#endif  // QKEYSTORAGECONTROLLERIMPL_H
