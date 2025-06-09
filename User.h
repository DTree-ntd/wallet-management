#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
private:
    std::string username;
    std::string password;
    std::string fullName;
    std::string email;
    std::string phoneNumber;
    bool isAdmin;
    bool isPasswordAutoGenerate;
    std::vector<std::string> walletIds;

public:
    // Constructors
    User();
    User(const std::string& username, const std::string& password, 
         const std::string& fullName, const std::string& email,
         const std::string& phoneNumber, bool isAdmin = false,
         bool isPasswordAutoGenerate = false);

    // Getters
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getFullName() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    bool getIsAdmin() const;
    bool getIsPasswordAutoGenerate() const;
    std::vector<std::string> getWalletIds() const;

    // Setters
    void setPassword(const std::string& newPassword);
    void setFullName(const std::string& newFullName);
    void setEmail(const std::string& newEmail);
    void setPhoneNumber(const std::string& newPhoneNumber);
    void setIsPasswordAutoGenerate(bool value);
    void addWalletId(const std::string& walletId);
};

#endif 