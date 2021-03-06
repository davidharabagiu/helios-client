#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <string>

/**
 * @class UserAccount
 * @brief Holds internal data for a user account
 */
class UserAccount
{
public:
    /**
     * @brief Constructor
     */
    UserAccount() = default;

    /**
     * @brief Constructor
     * @param username - User name
     * @param password - Password
     */
    UserAccount(const std::string& username, const std::string& password);

    /**
     * @brief operator ==
     * @param rhs - The other operand
     * @return bool
     */
    bool operator==(const UserAccount& rhs) const;

    /**
     * @brief operator !=
     * @param rhs - The other operand
     * @return bool
     */
    bool operator!=(const UserAccount& rhs) const;

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
     * @param newVal - std::string
     */
    void setUsername(const std::string& newVal);

    /**
     * @brief Getter for password
     * @return const std::string&
     */
    const std::string& password() const;

    /**
     * @brief Setter for password
     * @param newVal - std::string
     */
    void setPassword(const std::string& newVal);

private:
    /**
     * @brief User name
     */
    std::string m_username;

    /**
     * @brief Password
     */
    std::string m_password;
};

#endif  // USERACCOUNT_H
