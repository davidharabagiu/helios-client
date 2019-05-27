#ifndef QKEYSTORAGECONTROLLER_H
#define QKEYSTORAGECONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <memory>

#include "qkeystoragecontrollerimpl.h"

/**
 * @class QKeyStorageController
 * @brief Allows the user to create, view and use keys for the cryptographic ciphers that encrypt and decrypt his his
 * personal infromation
 */
class QKeyStorageController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Specify a key size in bit count
     */
    enum KeySize
    {
        /**
         * @brief 128-bit keys
         */
        KEY_SIZE_128 = static_cast<int>(QKeyStorageControllerImpl::KeySize::_128),

        /**
         * @brief 192-bit keys
         */
        KEY_SIZE_192 = static_cast<int>(QKeyStorageControllerImpl::KeySize::_192),

        /**
         * @brief 256-bit keys
         */
        KEY_SIZE_256 = static_cast<int>(QKeyStorageControllerImpl::KeySize::_256)
    };
    Q_ENUM(KeySize);

    /**
     * @brief Constructor
     * @param parent - Parent QObject
     */
    explicit QKeyStorageController(QObject* parent = nullptr);

    /**
     * @brief Returns all the names of the keys which are of a given size
     * @param keySize - enum KeySize
     * @return QStringList
     */
    Q_INVOKABLE QStringList keys(KeySize keySize) const;

    /**
     * @brief Create a new key
     * @param name - Key name
     * @param size - Key size
     * @return True if the key could be created (i.e. a key with the given name does not already exist)
     */
    Q_INVOKABLE bool createKey(const QString& name, KeySize size);

    /**
     * @brief Remove a key
     * @param name - Key name
     * @return True if the key could be removed (i.e. a key with the given name exists)
     */
    Q_INVOKABLE bool removeKey(const QString& name);

    /**
     * @brief Remove all keys
     */
    Q_INVOKABLE void removeAllKeys();

    /**
     * @brief Send a key to another user
     * @param username - Name of the destination user
     * @param keyName - Identity name of the key
     * @return True if a key with this name could be found and was sent
     */
    Q_INVOKABLE void sendKey(const QString& username, const QString& keyName);

signals:
    /**
     * @brief SIGNAL emitted when the list of keys has changed
     */
    void keysChanged();

private:
    /**
     * @brief Private implementation
     */
    std::shared_ptr<QKeyStorageControllerImpl> m_privateImpl;
};

#endif  // QKEYSTORAGECONTROLLER_H
