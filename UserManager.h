#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "User.h"

class UserManager {
private:
    std::map<std::string, User> users;
    std::string dataFile;

    bool isUsernameExists(const std::string& username);
    void sendLoginInfoToUser(const std::string& email, const std::string& username, 
                            const std::string& password);
User currentUser;
bool isLoggedIn = false;


public:
    UserManager(const std::string& usersPath);
    
    bool registerUser(const std::string& username, const std::string& password, 
                     const std::string& fullName, const std::string& email, 
                     const std::string& phoneNumber);
                     
    bool registerUserByAdmin(const std::string& username, const std::string& fullName,
                           const std::string& email, const std::string& phoneNumber);
                           
    bool login(const std::string& username, const std::string& password);
    std::string generateRandomPassword();
}; 
bool isUserLoggedIn() const { return isLoggedIn; }
User getCurrentUser() const { return currentUser; }

void setCurrentUser(const User& user) {
    currentUser = user;
    isLoggedIn = true;
}

void logout() {
    isLoggedIn = false;
}
