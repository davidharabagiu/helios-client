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

/**
 * @class KeyManagerImpl
 * @brief Private implementation of KeyManager
 */
class KeyManagerImpl : public KeyManager
{
public:
    /**
     * @brief Constructor
     * @param randomFactory - RNG factory
     */
    KeyManagerImpl(std::unique_ptr<RandomFactory> randomFactory);

public:  // from KeyManager
    std::vector<std::string> listKeys(uint16_t length) const override;
    bool                     createKey(const std::string& name, uint16_t length) override;
    void                     addKey(const std::string& name, const std::vector<uint8_t>& content) override;
    std::vector<uint8_t>     getKey(const std::string& name) const override;
    bool                     removeKey(const std::string& name) override;
    void                     removeAllKeys() override;

private:
    void persistKey(const QString& keyName, QFile& output) const;

private:
    /**
     * @brief Cached keys mapped by name
     */
    QMap<QString, QByteArray> m_keys;

    /**
     * @brief RNG instance
     */
    std::shared_ptr<Random> m_rng;
};

#endif  // KEYMANAGERIMPL_H
