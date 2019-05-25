#include <cstring>
#include <limits>

#include "keymanagerimpl.h"
#include "paths.h"
#include "typeconversions.h"
#include "random.h"
#include "randomfactory.h"

KeyManagerImpl::KeyManagerImpl(std::unique_ptr<RandomFactory> rngFactory)
    : m_rng(rngFactory->isaac64())
{
    QFile storageFile(QString::fromStdString(Paths::kKeyStorageFile));
    if (!storageFile.exists())
    {
        return;
    }

    storageFile.open(QIODevice::ReadOnly);
    QByteArray fileContent = storageFile.readAll();
    storageFile.close();

    for (int i = 0; i < fileContent.length();)
    {
        // Read key name
        QString keyName;
        while (fileContent.at(i) != '\0')
        {
            keyName += fileContent.at(i++);
        }

        // Read key size
        uint16_t keySize;
        std::memcpy(&keySize, fileContent.data() + ++i, sizeof(keySize));
        i += sizeof(keySize);

        // Read key content
        QByteArray keyContent = fileContent.mid(i, safe_integral_cast<int>(keySize));
        i += keySize;

        m_keys.insert(keyName, keyContent);
    }
}

std::vector<std::string> KeyManagerImpl::listKeys(uint16_t length) const
{
    std::vector<std::string> keyNames;
    keyNames.reserve(safe_integral_cast<size_t>(m_keys.count()));

    auto keyNamesQt = m_keys.keys();
    for (const auto& key : keyNamesQt)
    {
        if (safe_integral_cast<uint16_t>(m_keys.find(key)->length()) == length)
        {
            keyNames.push_back(key.toStdString());
        }
    }

    return keyNames;
}

bool KeyManagerImpl::createKey(const std::string& name, uint16_t length)
{
    if (m_keys.contains(QString::fromStdString(name)))
    {
        return false;
    }

    QByteArray key;
    key.reserve(length);
    for (uint16_t i = 0; i < length; ++i)
    {
        key.push_back(static_cast<char>(m_rng->get()));
    }

    QString keyName(QString::fromStdString(name));
    m_keys.insert(keyName, key);

    QFile storageFile(QString::fromStdString(Paths::kKeyStorageFile));
    storageFile.open(QIODevice::WriteOnly | QIODevice::Append);
    persistKey(keyName, storageFile);
    storageFile.close();

    return true;
}

std::vector<uint8_t> KeyManagerImpl::getKey(const std::string& name) const
{
    std::vector<uint8_t> result;

    auto it = m_keys.find(QString::fromStdString(name));
    if (it == m_keys.end())
    {
        return result;
    }

    result.resize(safe_integral_cast<size_t>(it->length()));
    std::copy(it->cbegin(), it->cend(), result.begin());

    return result;
}

bool KeyManagerImpl::removeKey(const std::string& name)
{
    bool success = static_cast<bool>(m_keys.remove(QString::fromStdString(name)));
    if (!success)
    {
        return success;
    }

    QFile storageFile(QString::fromStdString(Paths::kKeyStorageFile));
    storageFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    auto keyNames = m_keys.keys();
    for (const auto& key : keyNames)
    {
        persistKey(key, storageFile);
    }

    storageFile.close();
    return success;
}

void KeyManagerImpl::removeAllKeys()
{
    m_keys.clear();
    QFile(QString::fromStdString(Paths::kKeyStorageFile)).remove();
}

void KeyManagerImpl::persistKey(const QString& keyName, QFile& output) const
{
    static const char nullChar = '\0';

    auto it = m_keys.find(keyName);
    if (it == m_keys.cend())
    {
        return;
    }

    output.write(keyName.toUtf8());
    output.write(&nullChar, 1);
    uint16_t keyLength = safe_integral_cast<uint16_t>(it->length());
    output.write(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
    output.write(*it);
}
