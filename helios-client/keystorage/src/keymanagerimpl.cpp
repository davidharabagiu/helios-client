#include <cstring>
#include <limits>
#include <QCryptographicHash>

#include "keymanagerimpl.h"
#include "paths.h"
#include "typeconversions.h"
#include "random.h"
#include "randomfactory.h"
#include "cipher.h"
#include "cipherfactory.h"

const QByteArray KeyManagerImpl::s_kStorageValidityCheck = QByteArray::fromRawData("KEK", 3);

KeyManagerImpl::KeyManagerImpl(std::unique_ptr<RandomFactory> rngFactory, std::unique_ptr<CipherFactory> cipherFactory)
    : m_rng(rngFactory->isaac64())
    , m_cipher(cipherFactory->createCipher(CipherFactory::Algorithm::AES256, 1))
{
}

bool KeyManagerImpl::loadKeys(const std::string& username, const std::string& password)
{
    m_storagePath = Paths::kKeyStoragePath + "/" + username + ".keystorage";
    auto hashedPassword =
        QCryptographicHash::hash(QByteArray::fromStdString(password), QCryptographicHash::Algorithm::Sha3_256);
    m_encryptionKey = std::vector<uint8_t>(hashedPassword.begin(), hashedPassword.end());

    QFile storageFile(QString::fromStdString(m_storagePath));
    if (!storageFile.exists())
    {
        return true;
    }
    storageFile.open(QIODevice::ReadOnly);
    QByteArray encryptedStorage = storageFile.readAll();
    storageFile.close();

    QByteArray decryptedStorage;
    uint32_t   realStorageSize;
    auto       paddedStorageSize = encryptedStorage.size() - static_cast<int>(sizeof(realStorageSize));
    std::memcpy(&realStorageSize, encryptedStorage.data(), sizeof(realStorageSize));
    decryptedStorage.resize(paddedStorageSize);

    m_cipher->setKey(m_encryptionKey.data());
    m_cipher->decrypt(reinterpret_cast<const uint8_t*>(encryptedStorage.data() + sizeof(realStorageSize)),
                      safe_integral_cast<uint64_t>(paddedStorageSize),
                      reinterpret_cast<uint8_t*>(decryptedStorage.data()));

    if (decryptedStorage.mid(0, s_kStorageValidityCheck.size()) != s_kStorageValidityCheck)
    {
        return false;
    }

    decryptedStorage.resize(static_cast<int>(realStorageSize));

    for (int i = s_kStorageValidityCheck.length(); i < decryptedStorage.length();)
    {
        // Read key name
        QString keyName;
        while (decryptedStorage.at(i) != '\0')
        {
            keyName += decryptedStorage.at(i++);
        }

        // Read key size
        uint16_t keySize;
        std::memcpy(&keySize, decryptedStorage.data() + ++i, sizeof(keySize));
        i += safe_integral_cast<int>(sizeof(keySize));

        // Read key content
        QByteArray keyContent = decryptedStorage.mid(i, safe_integral_cast<int>(keySize));
        i += keySize;

        m_keys.insert(keyName, keyContent);
    }

    return true;
}

void KeyManagerImpl::unloadKeys()
{
    m_keys.clear();
    m_encryptionKey.clear();
    m_storagePath.clear();
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

    persistKeys();

    return true;
}

void KeyManagerImpl::addKey(const std::string& name, const std::vector<uint8_t>& content)
{
    QString actualName;
    for (int count = 0;
         m_keys.contains(actualName = QString::fromStdString(count ? name + std::to_string(count) : name)); ++count)
        ;

    auto key =
        QByteArray::fromRawData(reinterpret_cast<const char*>(content.data()), safe_integral_cast<int>(content.size()));
    key.detach();

    m_keys.insert(actualName, key);

    persistKeys();
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

    persistKeys();

    return success;
}

void KeyManagerImpl::removeAllKeys()
{
    m_keys.clear();
    QFile(QString::fromStdString(m_storagePath)).remove();
}

void KeyManagerImpl::persistKeys() const
{
    QByteArray decryptedStorage = s_kStorageValidityCheck;
    for (auto it = m_keys.constKeyValueBegin(); it != m_keys.constKeyValueEnd(); ++it)
    {
        decryptedStorage.push_back((*it).first.toUtf8());
        decryptedStorage.push_back('\0');
        uint16_t keyLength = safe_integral_cast<uint16_t>((*it).second.length());
        decryptedStorage.push_back(QByteArray::fromRawData(reinterpret_cast<char*>(&keyLength), sizeof(keyLength)));
        decryptedStorage.push_back((*it).second);
    }

    auto paddedStorageSize = ((decryptedStorage.size() % safe_integral_cast<int>(m_cipher->blockSize()) == 0) ?
                                  decryptedStorage.size() :
                                  (decryptedStorage.size() / safe_integral_cast<int>(m_cipher->blockSize()) + 1) *
                                      safe_integral_cast<int>(m_cipher->blockSize()));

    auto realStorageSize = decryptedStorage.size();

    QByteArray encryptedStorage;
    encryptedStorage.resize(safe_integral_cast<int>(sizeof(realStorageSize)) + paddedStorageSize);
    std::memcpy(encryptedStorage.data(), &realStorageSize, sizeof(realStorageSize));
    m_cipher->setKey(m_encryptionKey.data());
    m_cipher->encrypt(reinterpret_cast<const uint8_t*>(decryptedStorage.data()),
                      safe_integral_cast<uint64_t>(realStorageSize),
                      reinterpret_cast<uint8_t*>(encryptedStorage.data() + sizeof(realStorageSize)));

    QFile storageFile(QString::fromStdString(m_storagePath));
    storageFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    storageFile.write(encryptedStorage);
    storageFile.close();
}
