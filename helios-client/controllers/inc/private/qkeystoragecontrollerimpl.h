#ifndef QKEYSTORAGECONTROLLERIMPL_H
#define QKEYSTORAGECONTROLLERIMPL_H

#include <QList>
#include <QString>
#include <memory>
#include <cstdint>

// Forward declarations
class KeyManager;

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
     */
    QKeyStorageControllerImpl();

public:  // Forwarded from QKeyStorageController
    QList<QString> keys(KeySize keySize) const;
    bool           createKey(const QString& name, KeySize size);
    bool           removeKey(const QString& name);
    void           removeAllKeys();

private:
    static uint16_t keySizeToByteLength(KeySize keySize);

private:
    /**
     * @brief Key manager instance
     */
    std::shared_ptr<KeyManager> m_keyManager;
};

#endif  // QKEYSTORAGECONTROLLERIMPL_H
