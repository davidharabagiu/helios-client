#ifndef QKEYSTORAGECONTROLLERIMPL_H
#define QKEYSTORAGECONTROLLERIMPL_H

#include <QList>
#include <QString>

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
};

#endif  // QKEYSTORAGECONTROLLERIMPL_H
