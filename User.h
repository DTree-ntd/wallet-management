#ifndef USER_H
#define USER_H
#include "Wallet.h"
#include <vector>
#include <string>
#include <vector>

class User {
private:
    std::vector<Wallet> wallets;
    std::string username;
    std::string password;
    std::string fullName;
    std::string email;
    std::string phoneNumber;
    bool isAdmin;
    bool isPasswordAutoGenerate;
    std::vector<std::string> walletIds;

public:
    // Thêm ví mới cho user, với walletId tự tạo hoặc truyền vào
    void addWallet(const Wallet& wallet);
    // Lấy danh sách ví
    std::vector<Wallet>& getWallets();
    // Tìm ví theo walletId, trả về con trỏ nullptr nếu không tìm thấy
    Wallet* findWalletById(const std::string& walletId);

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
    void addWalletId(const std::string& walletId);
};

#endif 
