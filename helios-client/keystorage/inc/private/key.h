#ifndef KEY_H
#define KEY_H

#include <string>
#include <cstdint>
#include <vector>

/**
 * @class Key
 * @brief Models a cipher key for KeyManagerImpl
 */
class Key
{
public:
    /**
     * @brief Constructs a random key
     * @param name - Key name
     * @param length - Key length in bytes
     */
    Key(const std::string& name, uint16_t length);

    /**
     * @brief Constructs a key with the given content
     * @param name - Key name
     * @param length - Key length in bytes
     * @param content - Key content
     */
    Key(const std::string& name, uint16_t length, const uint8_t* content);

    /**
     * @brief Copy constructor
     * @param rhs - Other Key
     */
    Key(const Key& rhs);

    /**
     * @brief Copy assignment
     * @param rhs - Other Key
     * @return *this
     */
    Key& operator=(const Key& rhs);

    /**
     * @brief Getter for m_name
     * @return std::string
     */
    std::string name() const;

    /**
     * @brief Returns the length of the key in bytes
     * @return uint16_t
     */
    uint16_t length() const;

    /**
     * @brief Returns the content of the key
     * @return const uint8_t*
     */
    const uint8_t* content() const;

private:
    /**
     * @brief Key name
     */
    std::string m_name;

    /**
     * @brief Actual key content
     * @note Workaround because some compilers don't support a shared_ptr to an array
     */
    std::vector<uint8_t> m_content;
};

#endif  // KEY_H
