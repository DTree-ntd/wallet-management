#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/stat.h>

UserManager::UserManager(const std::string& usersPath)
    : dataFile(usersPath) {
    // Tạo thư mục data nếu chưa tồn tại
    std::string dataDir = usersPath.substr(0, usersPath.find_last_of("/\\"));
    struct stat info;
    if (stat(dataDir.c_str(), &info) != 0) {
        #ifdef _WIN32
        system(("mkdir \"" + dataDir + "\"").c_str());
        #else
        system(("mkdir -p \"" + dataDir + "\"").c_str());
        #endif
    }
    
    // Tạo file users.txt nếu chưa tồn tại
    std::ifstream checkFile(dataFile);
    if (!checkFile.good()) {
        std::ofstream createFile(dataFile);
        if (!createFile.is_open()) {
            std::cout << "Loi: Khong the tao file users.txt!\n";
        }
        createFile.close();
    }
    checkFile.close();
}

bool UserManager::isUsernameExists(const std::string& username) {
    std::ifstream file(dataFile);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string existingUsername;
        std::getline(ss, existingUsername, '|');
        if (existingUsername == username) {
            return true;
        }
    }
    return false;
}

bool UserManager::registerUser(const std::string& username, const std::string& password,
                             const std::string& fullName, const std::string& email,
                             const std::string& phoneNumber) {
    if (isUsernameExists(username)) {
        return false;
    }

    // Lưu thông tin user vào file
    std::ofstream file(dataFile, std::ios::app);
    if (!file.is_open()) {
        return false;
    }

    file << username << "|" << password << "|" 
         << fullName << "|" << email << "|" << phoneNumber << "|0|0\n";
    file.close();

    return true;
}

bool UserManager::login(const std::string& username, const std::string& password) {
    std::ifstream file(dataFile);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string storedUsername, storedPassword;
        std::getline(ss, storedUsername, '|');
        std::getline(ss, storedPassword, '|');

        if (storedUsername == username && storedPassword == password) {
            return true;
        }
    }
    return false;
}

std::string UserManager::generateRandomPassword() {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string password;
    std::srand(std::time(nullptr));
    for (int i = 0; i < 8; i++) {
        password += chars[std::rand() % chars.length()];
    }
    return password;
}

void UserManager::sendLoginInfoToUser(const std::string& email, const std::string& username, 
                                    const std::string& password) {
    // TODO: Implement email sending
    std::cout << "Gui thong tin dang nhap den " << email << std::endl;
    std::cout << "Ten dang nhap: " << username << std::endl;
    std::cout << "Mat khau: " << password << std::endl;
}

bool UserManager::registerUserByAdmin(const std::string& username, const std::string& fullName,
                                    const std::string& email, const std::string& phoneNumber) {
    if (username.empty() || fullName.empty() || 
        email.empty() || phoneNumber.empty()) {
        std::cout << "Loi: Vui long nhap day du thong tin!\n";
        return false;
    }

    if (isUsernameExists(username)) {
        std::cout << "Loi: Ten dang nhap da ton tai!\n";
        return false;
    }

    std::string autoPassword = generateRandomPassword();

    std::ofstream userFile(dataFile, std::ios::app);
    if (!userFile.is_open()) {
        std::cout << "Loi: Khong the mo file de ghi!\n";
        return false;
    }
    
    userFile << username << "|"
            << autoPassword << "|"
            << fullName << "|"
            << email << "|"
            << phoneNumber << "|"
            << "false|" // isAdmin
            << "true|" // isPasswordAutoGenerate
            << "\n"; // walletIds (empty for now)
    userFile.close();

    sendLoginInfoToUser(email, username, autoPassword);
    return true;
} 