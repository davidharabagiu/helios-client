#ifndef USERSESSION_H
#define USERSESSION_H

#include <string>

/**
 * @class UserSession
 * @brief Models a user authentication session
 */
class UserSession
{
public:
    /**
     * @brief Constructor
     */
    UserSession() = default;

    /**
     * @brief Constructor
     * @param username - User name
     * @param password - Password
     */
    UserSession(const std::string& username, const std::string& authToken);

    /**
     * @brief operator ==
     * @param rhs - The other operand
     * @return bool
     */
    bool operator==(const UserSession& rhs) const;

    /**
     * @brief operator !=
     * @param rhs - The other operand
     * @return bool
     */
    bool operator!=(const UserSession& rhs) const;

    /**
     * @brief Clear all data
     */
    void clear();

    /**
     * @brief Validity check
     * @return bool
     */
    bool valid() const;

    /**
     * @brief Getter for username
     * @return const std::string&
     */
    const std::string& username() const;

    /**
     * @brief Setter for username
     * @param newVal - const std::string&
     */
    void setUsername(const std::string& newVal);

    /**
     * @brief Getter for authentication token
     * @return const std::string&
     */
    const std::string& authToken() const;

    /**
     * @brief Setter for authentication token
     * @param newVal - const std::string&
     */
    void setAuthToken(const std::string& newVal);

private:
    /**
     * @brief User name
     */
    std::string m_username;

    /**
     * @brief Authentication token
     */
    std::string m_authToken;
};

#endif  // USERSESSION_H
