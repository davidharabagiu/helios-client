#ifndef CONFIG_H
#define CONFIG_H

#include <any>
#include <string>

/**
 * @class Config
 * @brief Provides application configurations
 */
class Config
{
public:
    /**
     * @brief Destructor
     */
    virtual ~Config() = default;

    /**
     * @brief Returns a config value. If the value with the given key could not be found, it returns an empty object.
     * @param key - Config key
     * @return std::any
     */
    virtual std::any get(const std::string& key) const = 0;
};

#endif  // CONFIG_H
