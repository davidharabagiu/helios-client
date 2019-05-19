#ifndef KEYMANAGERIMPL_H
#define KEYMANAGERIMPL_H

#include <cstdint>
#include <QMap>
#include <QByteArray>
#include <QString>
#include <QFile>

#include "keymanager.h"

/**
 * @class KeyManagerImpl
 * @brief Private implementation of KeyManager
 */
class KeyManagerImpl : public KeyManager
{
public:
    KeyManagerImpl();

public:  // from KeyManager
    std::vector<std::string> listKeys(uint16_t length) const;
    bool                     createKey(const std::string& name, uint16_t length);
    std::vector<uint8_t>     getKey(const std::string& name) const;
    bool                     removeKey(const std::string& name);
    void                     removeAllKeys();

private:
    void persistKey(const QString& keyName, QFile& output) const;

private:
    /**
     * @brief Cached keys mapped by name
     */
    QMap<QString, QByteArray> m_keys;
};

#endif  // KEYMANAGERIMPL_H
