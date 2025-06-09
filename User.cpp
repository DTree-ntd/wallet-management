#include "User.h"

User::User() : isAdmin(false), isPasswordAutoGenerate(false) {}

User::User(const std::string& username, const std::string& password, 
           const std::string& fullName, const std::string& email,
           const std::string& phoneNumber, bool isAdmin,
           bool isPasswordAutoGenerate)
    : username(username), password(password), fullName(fullName),
      email(email), phoneNumber(phoneNumber), isAdmin(isAdmin),
      isPasswordAutoGenerate(isPasswordAutoGenerate) {
    
    // Tạo ví cho user với ID là username
    wallet = Wallet(username);
}

// Getters
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getFullName() const { return fullName; }
std::string User::getEmail() const { return email; }
std::string User::getPhoneNumber() const { return phoneNumber; }
bool User::getIsAdmin() const { return isAdmin; }
bool User::getIsPasswordAutoGenerate() const { return isPasswordAutoGenerate; }

// Setters
void User::setPassword(const std::string& newPassword) { password = newPassword; }
void User::setFullName(const std::string& newFullName) { fullName = newFullName; }
void User::setEmail(const std::string& newEmail) { email = newEmail; }
void User::setPhoneNumber(const std::string& newPhoneNumber) { phoneNumber = newPhoneNumber; }
void User::setIsPasswordAutoGenerate(bool value) {
    isPasswordAutoGenerate = value;
} 