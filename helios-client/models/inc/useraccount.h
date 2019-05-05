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

public:  // forwarded public methods
    const std::string& username() const;
    void               setUsername(const std::string& newVal);
    const std::string& password() const;
    void               setPassword(const std::string& newVal);

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
