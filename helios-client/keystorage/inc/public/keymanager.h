#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <string>
#include <cstdint>

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
     * @brief Create and store a random key
     * @param name - Name of the new key
     * @param length - Length of the key in bytes
     * @return False if the key could not be created because a key with this name already exists
     */
    virtual bool createKey(const std::string& name, unsigned length) = 0;

    /**
     * @brief Retrieve an existent key
     * @param name - Name of the key
     * @param key - Output parameter, the requested key
     * @return False is a key with this name does not exist
     */
    virtual bool getKey(const std::string& name, uint8_t* key) = 0;

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
