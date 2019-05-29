#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <string>
#include <cstdint>
#include <vector>
#include <memory>

/**
 * @class KeyManager
 * @brief Provides data encryption key management and storage
 */
class KeyManager
{
public:
    /**
     * @brief Destructor
     */
    virtual ~KeyManager() = default;

    /**
     * @brief Decrypt and load the user local key storage
     * @param username - Name of the user
     * @param password - Password of the user used for decryption
     * @return True if the storage could be decrypted using the given password
     */
    virtual bool loadKeys(const std::string& username, const std::string& password) = 0;

    /**
     * @brief Unload all keys from memory
     */
    virtual void unloadKeys() = 0;

    /**
     * @brief Get all key names which have a given length
     * @param length - Length of the keys in bytes
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> listKeys(uint16_t length) const = 0;

    /**
     * @brief Create and store a random key
     * @param name - Name of the new key
     * @param length - Length of the key in bytes
     * @return False if the key could not be created because a key with this name already exists
     */
    virtual bool createKey(const std::string& name, uint16_t length) = 0;

    /**
     * @brief Add a key with an existing content. If a key with this name already exists, numbers may be appended to the
     * end of the key in order to solve this conflict.
     * @param name - Required name of the key
     * @param content - Content of the key
     */
    virtual void addKey(const std::string& name, const std::vector<uint8_t>& content) = 0;

    /**
     * @brief Retrieve an existent key
     * @param name - Name of the key
     * @return The requested key. The vector has size zero if the key with the given name does not exist.
     */
    virtual std::vector<uint8_t> getKey(const std::string& name) const = 0;

    /**
     * @brief Remove an existent key
     * @param name - Name of the key
     * @return False is a key with this name does not exist
     */
    virtual bool removeKey(const std::string& name) = 0;

    /**
     * @brief Remove all keys
     */
    virtual void removeAllKeys() = 0;
};

#endif  // KEYMANAGER_H
