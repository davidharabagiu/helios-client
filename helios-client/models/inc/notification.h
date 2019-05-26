#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

/**
 * @class Notification
 * @brief Internal representation of a user notification
 */
class Notification
{
public:
    enum class Type
    {
        FILE_SHARE,
        KEY_SHARE
    };

    /**
     * @brief Constructor
     * @param id - Notification id
     * @param text - Notification text / content
     * @param type - Notification type
     */
    Notification(const std::string& id, const std::string& text, Type type);

    /**
     * @brief Getter for m_id
     * @return std::string
     */
    std::string id() const;

    /**
     * @brief Getter for m_text
     * @return std::string
     */
    std::string text() const;

    /**
     * @brief Getter for m_type
     * @return Notification::Type
     */
    Type type() const;

private:
    /**
     * @brief Notification id
     */
    std::string m_id;

    /**
     * @brief Notification text / content
     */
    std::string m_text;

    /**
     * @brief Notification type
     */
    Type m_type;
};

#endif  // NOTIFICATION_H
