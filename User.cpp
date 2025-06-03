#include "User.h"

User::User() : isAdmin(false), isPasswordAutoGenerate(false) {}

User::User(const std::string& username, const std::string& password, 
           const std::string& fullName, const std::string& email,
           const std::string& phoneNumber, bool isAdmin,
           bool isPasswordAutoGenerate)
    : username(username), password(password), fullName(fullName),
      email(email), phoneNumber(phoneNumber), isAdmin(isAdmin),
      isPasswordAutoGenerate(isPasswordAutoGenerate) {}

// Getters
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getFullName() const { return fullName; }
std::string User::getEmail() const { return email; }
std::string User::getPhoneNumber() const { return phoneNumber; }
bool User::getIsAdmin() const { return isAdmin; }
bool User::getIsPasswordAutoGenerate() const { return isPasswordAutoGenerate; }
std::vector<std::string> User::getWalletIds() const { return walletIds; }

// Setters
void User::setPassword(const std::string& newPassword) { password = newPassword; }
void User::setFullName(const std::string& newFullName) { fullName = newFullName; }
void User::setEmail(const std::string& newEmail) { email = newEmail; }
void User::setPhoneNumber(const std::string& newPhoneNumber) { phoneNumber = newPhoneNumber; }
void User::addWalletId(const std::string& walletId) { walletIds.push_back(walletId); } 

void User::addWallet(const Wallet& wallet) {
    wallets.push_back(wallet);
}

std::vector<Wallet>& User::getWallets() {
    return wallets;
}

Wallet* User::findWalletById(const std::string& walletId) {
    for (auto& w : wallets) {
        if (w.getWalletId() == walletId) {
            return &w;
        }
    }
    return nullptr;
}
