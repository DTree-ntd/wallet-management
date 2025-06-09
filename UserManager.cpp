#include "UserManager.h"
#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/stat.h>
#include <regex>
#include <set>

// Define static data member
const std::string UserManager::TOTAL_WALLET_ID = "TOTAL_WALLET_001";

UserManager::UserManager(const std::string& usersPath)
    : dataFile(usersPath), isSystemInitialized(false) {
    // Create data directory if it doesn't exist
    std::string dataDir = usersPath.substr(0, usersPath.find_last_of("/\\"));
    struct stat info;
    if (stat(dataDir.c_str(), &info) != 0) {
        #ifdef _WIN32
        system(("mkdir \"" + dataDir + "\"").c_str());
        #else
        system(("mkdir -p \"" + dataDir + "\"").c_str());
        #endif
    }
    
    // Create users.txt file if it doesn't exist
    std::ifstream checkFile(dataFile);
    if (!checkFile.good()) {
        std::ofstream createFile(dataFile);
        if (!createFile.is_open()) {
            std::cout << "Loi: Khong the tao file users.txt!\n";
            return;
        }
        
        // Create admin account
        std::string adminUsername = "admin";
        std::string adminPassword = "password"; // Default password
        std::string adminFullName = "System Administrator";
        std::string adminEmail = "admin@system.com";
        std::string adminPhone = "0123456789";
        
        // Hash password
        std::string hashedPassword = PasswordHasher::hashPassword(adminPassword);
        
        // Create admin user
        User adminUser(adminUsername, hashedPassword, adminFullName, adminEmail, adminPhone, true, false);
        users[adminUsername] = adminUser;
        
        // Save to file
        saveToFile(dataFile);
        createFile.close();
    }
    checkFile.close();

    // Load users data from file
    loadFromFile(dataFile);

    // Initialize total wallet
    initializeSystem();
}

bool UserManager::saveToFile(const std::string& filename) const {
    // Lưu thông tin users
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Lưu users theo thứ tự để tránh trùng lặp
    std::map<std::string, User> sortedUsers(users.begin(), users.end());
    for (const auto& user : sortedUsers) {
        file << user.second.getUsername() << ","
             << user.second.getPassword() << ","
             << user.second.getFullName() << ","
             << user.second.getEmail() << ","
             << user.second.getPhoneNumber() << ","
             << user.second.getIsAdmin() << ","
             << user.second.getIsPasswordAutoGenerate() << "\n";
    }
    file.close();

    // Lưu thông tin các ví
    std::string dataDir = filename.substr(0, filename.find_last_of("/\\"));
    std::string walletsPath = dataDir + "/wallets.txt";
    std::ofstream walletsFile(walletsPath);
    if (walletsFile.is_open()) {
        // Lưu ví tổng đầu tiên
        walletsFile << TOTAL_WALLET_ID << ","
                    << totalWallet.getBalance() << ","
                    << static_cast<int>(WalletType::TOTAL) << "\n";
        
        // Chỉ lưu ví của admin
        auto adminIt = sortedUsers.find("admin");
        if (adminIt != sortedUsers.end()) {
            const Wallet& wallet = adminIt->second.getWallet();
            walletsFile << wallet.getWalletId() << ","
                       << wallet.getBalance() << ","
                       << static_cast<int>(WalletType::USER) << "\n";
        }
        walletsFile.close();
    }

    // Lưu lịch sử giao dịch
    std::string transactionsPath = dataDir + "/transactions.txt";
    std::ofstream transactionsFile(transactionsPath);
    if (transactionsFile.is_open()) {
        // Tạo một set để theo dõi các giao dịch đã được lưu
        std::set<std::string> savedTransactionIds;
        
        // Lưu giao dịch của ví tổng
        for (const auto& trans : totalWallet.getTransactionHistory()) {
            transactionsFile << trans.transactionId << ","
                           << trans.fromWalletId << ","
                           << trans.toWalletId << ","
                           << trans.points << ","
                           << trans.timestamp << ","
                           << trans.status << "\n";
            savedTransactionIds.insert(trans.transactionId);
        }
        
        // Lưu giao dịch của admin (chỉ những giao dịch chưa được lưu)
        auto adminIt = sortedUsers.find("admin");
        if (adminIt != sortedUsers.end()) {
            for (const auto& trans : adminIt->second.getWallet().getTransactionHistory()) {
                if (savedTransactionIds.find(trans.transactionId) == savedTransactionIds.end()) {
                    transactionsFile << trans.transactionId << ","
                                   << trans.fromWalletId << ","
                                   << trans.toWalletId << ","
                                   << trans.points << ","
                                   << trans.timestamp << ","
                                   << trans.status << "\n";
                    savedTransactionIds.insert(trans.transactionId);
                }
            }
        }
        transactionsFile.close();
    }

    return true;
}

void UserManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    std::map<std::string, User> loadedUsers; // Sử dụng map để tránh trùng lặp

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password, fullName, email, phoneNumber;
        bool isAdmin, isPasswordAutoGenerate;
        char comma;

        if (!(ss >> username >> comma >> password >> comma >> fullName >> comma >> 
              email >> comma >> phoneNumber >> comma >> isAdmin >> comma >> isPasswordAutoGenerate)) {
            continue; // Bỏ qua dòng không hợp lệ
        }

        // Chỉ lưu user đầu tiên nếu là admin
        if (username == "admin" && loadedUsers.find("admin") != loadedUsers.end()) {
            continue;
        }

        User user(username, password, fullName, email, phoneNumber, isAdmin);
        user.setIsPasswordAutoGenerate(isPasswordAutoGenerate);
        loadedUsers[username] = user;
    }

    // Cập nhật users map với dữ liệu đã load
    users = std::move(loadedUsers);
    
    // Khởi tạo ví tổng
    initializeSystem();
}

bool UserManager::isUsernameExists(const std::string& username) {
    return users.find(username) != users.end();
}

bool UserManager::isValidEmail(const std::string& email) {
    // Pattern for email: example@domain.com
    const std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return std::regex_match(email, pattern);
}

bool UserManager::isValidPhoneNumber(const std::string& phoneNumber) {
    // Pattern for phone number: 10 digits
    const std::regex pattern("^[0-9]{10}$");
    return std::regex_match(phoneNumber, pattern);
}

RegisterResult UserManager::registerUser(const std::string& username, const std::string& password, 
                                      const std::string& fullName, const std::string& email, 
                                      const std::string& phoneNumber, bool isPasswordAutoGenerated) {
    // Check username
    if (username.empty()) {
        return RegisterResult::INVALID_USERNAME;
    }
    if (isUsernameExists(username)) {
        return RegisterResult::USERNAME_EXISTS;
    }

    // Check password (if not auto-generated)
    if (!isPasswordAutoGenerated && password.empty()) {
        return RegisterResult::INVALID_PASSWORD;
    }

    // Validate email using regex
    if (!isValidEmail(email)) {
        return RegisterResult::INVALID_EMAIL;
    }

    // Validate phone number using regex
    if (!isValidPhoneNumber(phoneNumber)) {
        return RegisterResult::INVALID_PHONE;
    }

    // Hash the password
    std::string hashedPassword;
    try {
        hashedPassword = PasswordHasher::hashPassword(password);
    } catch (const std::exception& e) {
        std::cerr << "Error hashing password: " << e.what() << std::endl;
        return RegisterResult::FILE_ERROR;
    }

    // Create new user (constructor sẽ tự động tạo ví)
    User newUser(username, hashedPassword, fullName, email, phoneNumber, false, isPasswordAutoGenerated);
    users[username] = newUser;

    // Save to file
    if (!saveToFile(dataFile)) {
        users.erase(username); // Remove user from map if file save fails
        return RegisterResult::FILE_ERROR;
    }

    return RegisterResult::SUCCESS;
}

bool UserManager::login(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }

    return PasswordHasher::verifyPassword(password, it->second.getPassword());
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

std::string UserManager::setupOTP(const std::string& username) {
    // Generate a temporary OTP
    std::string otpCode = otpManager.generateTempOTP(username);
    if (otpCode.empty()) {
        return "";
    }
    
    return otpCode;
}

bool UserManager::verifyOTP(const std::string& username, const std::string& otp) {
    return otpManager.verifyTempOTP(username, otp);
}

bool UserManager::isPasswordAutoGenerated(const std::string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        return it->second.getIsPasswordAutoGenerate();
    }
    return false;
}

bool UserManager::changePassword(const std::string& username, const std::string& oldPassword, 
                               const std::string& newPassword) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }

    // Verify old password
    if (!PasswordHasher::verifyPassword(oldPassword, it->second.getPassword())) {
        return false;
    }

    // Hash new password
    std::string hashedNewPassword;
    try {
        hashedNewPassword = PasswordHasher::hashPassword(newPassword);
    } catch (const std::exception& e) {
        std::cerr << "Error hashing new password: " << e.what() << std::endl;
        return false;
    }

    // Update password and set isPasswordAutoGenerate to false
    it->second.setPassword(hashedNewPassword);
    it->second.setIsPasswordAutoGenerate(false);

    // Save changes
    if (!saveToFile(dataFile)) {
        return false;
    }

    return true;
}

std::string UserManager::initiatePasswordChange(const std::string& username) {
    auto it = users.find(username);
    if (it == users.end()) {
        return "";
    }
    
    // Generate temporary OTP
    std::string secretKey = otpManager.generateTempOTP(username);
    if (secretKey.empty()) {
        return "";
    }
    
    // Get the current OTP code
    std::string otpCode = otpManager.getCurrentOTP();
    if (otpCode.empty()) {
        otpManager.removeTempOTP(username);
        return "";
    }
    
    return otpCode;
}

bool UserManager::changePasswordWithOTP(const std::string& username, const std::string& oldPassword, 
                                      const std::string& newPassword, const std::string& otp) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }

    // Verify old password
    if (!PasswordHasher::verifyPassword(oldPassword, it->second.getPassword())) {
        return false;
    }

    // Verify OTP
    if (!otpManager.verifyTempOTP(username, otp)) {
        otpManager.removeTempOTP(username);
        return false;
    }

    // Hash new password
    std::string hashedNewPassword;
    try {
        hashedNewPassword = PasswordHasher::hashPassword(newPassword);
    } catch (const std::exception& e) {
        std::cerr << "Error hashing new password: " << e.what() << std::endl;
        otpManager.removeTempOTP(username);
        return false;
    }

    // Update password
    it->second.setPassword(hashedNewPassword);
    it->second.setIsPasswordAutoGenerate(false);

    // Save changes
    if (!saveToFile(dataFile)) {
        otpManager.removeTempOTP(username);
        return false;
    }

    // Remove temporary OTP after successful password change
    otpManager.removeTempOTP(username);
    return true;
}

std::string UserManager::getCurrentOTP() {
    return otpManager.getCurrentOTP();
}

void UserManager::printOTPQRCode(const std::string& otp) {
    otpManager.printQRCode(otp);
}

std::string UserManager::initiateUserInfoUpdate(const std::string& username) {
    auto it = users.find(username);
    if (it == users.end()) {
        return "";
    }
    
    // Generate temporary OTP
    std::string otpCode = otpManager.generateTempOTP(username);
    if (otpCode.empty()) {
        return "";
    }
    
    return otpCode;
}

bool UserManager::updateUserInfoWithOTP(const std::string& username,
                                      const std::string& newFullName,
                                      const std::string& newEmail,
                                      const std::string& newPhoneNumber,
                                      const std::string& otp) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }

    // Verify OTP
    if (!otpManager.verifyTempOTP(username, otp)) {
        return false;
    }

    // Validate new email if changed
    if (newEmail != it->second.getEmail() && !isValidEmail(newEmail)) {
        return false;
    }

    // Validate new phone number if changed
    if (newPhoneNumber != it->second.getPhoneNumber() && !isValidPhoneNumber(newPhoneNumber)) {
        return false;
    }

    // Update user information
    it->second.setFullName(newFullName);
    it->second.setEmail(newEmail);
    it->second.setPhoneNumber(newPhoneNumber);

    // Save changes
    if (!saveToFile(dataFile)) {
        return false;
    }

    return true;
}

bool UserManager::isAdmin(const std::string& username) const {
    auto it = users.find(username);
    if (it != users.end()) {
        return it->second.getIsAdmin();
    }
    return false;
}

std::vector<User> UserManager::getAllUsers() const {
    std::vector<User> userList;
    for (const auto& pair : users) {
        userList.push_back(pair.second);
    }
    return userList;
}

RegisterResult UserManager::createUserByAdmin(const std::string& username, const std::string& password,
                                           const std::string& fullName, const std::string& email,
                                           const std::string& phoneNumber, bool isAdmin) {
    // Check username
    if (username.empty()) {
        return RegisterResult::INVALID_USERNAME;
    }
    if (isUsernameExists(username)) {
        return RegisterResult::USERNAME_EXISTS;
    }

    // Check password
    if (password.empty()) {
        return RegisterResult::INVALID_PASSWORD;
    }

    // Validate email
    if (!isValidEmail(email)) {
        return RegisterResult::INVALID_EMAIL;
    }

    // Validate phone number
    if (!isValidPhoneNumber(phoneNumber)) {
        return RegisterResult::INVALID_PHONE;
    }

    // Hash the password
    std::string hashedPassword;
    try {
        hashedPassword = PasswordHasher::hashPassword(password);
    } catch (const std::exception& e) {
        std::cerr << "Error hashing password: " << e.what() << std::endl;
        return RegisterResult::FILE_ERROR;
    }

    // Create new user with admin flag
    User newUser(username, hashedPassword, fullName, email, phoneNumber, isAdmin, false);
    users[username] = newUser;

    // Save to file
    if (!saveToFile(dataFile)) {
        users.erase(username);
        return RegisterResult::FILE_ERROR;
    }

    return RegisterResult::SUCCESS;
}

bool UserManager::updateUserInfoByAdmin(const std::string& targetUsername,
                                      const std::string& newFullName,
                                      const std::string& newEmail,
                                      const std::string& newPhoneNumber) {
    auto it = users.find(targetUsername);
    if (it == users.end()) {
        return false;
    }

    // Validate new email if changed
    if (newEmail != it->second.getEmail() && !isValidEmail(newEmail)) {
        return false;
    }

    // Validate new phone number if changed
    if (newPhoneNumber != it->second.getPhoneNumber() && !isValidPhoneNumber(newPhoneNumber)) {
        return false;
    }

    // Update user information
    it->second.setFullName(newFullName);
    it->second.setEmail(newEmail);
    it->second.setPhoneNumber(newPhoneNumber);

    // Save changes
    if (!saveToFile(dataFile)) {
        return false;
    }

    return true;
}

void UserManager::initializeSystem() {
    if (!isSystemInitialized) {
        std::string dataDir = dataFile.substr(0, dataFile.find_last_of("/\\"));
        
        // Kiểm tra và tạo thư mục data nếu chưa tồn tại
        struct stat info;
        if (stat(dataDir.c_str(), &info) != 0) {
            #ifdef _WIN32
            system(("mkdir " + dataDir).c_str());
            #else
            system(("mkdir -p " + dataDir).c_str());
            #endif
        }

        // Kiểm tra và tạo admin nếu chưa tồn tại
        if (users.find("admin") == users.end()) {
            std::string hashedPassword = PasswordHasher::hashPassword("password");
            User admin("admin", hashedPassword, "System Administrator", "admin@system.com", "0123456789", true);
            admin.setIsPasswordAutoGenerate(false);
            users["admin"] = admin;
        }
        
        // Create wallets file if it doesn't exist
        std::string walletsPath = dataDir + "/wallets.txt";
        std::ifstream checkWallets(walletsPath);
        if (!checkWallets.good()) {
            std::ofstream createWallets(walletsPath);
            if (!createWallets.is_open()) {
                std::cout << "Loi: Khong the tao file wallets.txt!\n";
                return;
            }
            
            // Initialize total wallet with 0 balance
            totalWallet = Wallet(TOTAL_WALLET_ID, 0, WalletType::TOTAL);
            createWallets << totalWallet.getWalletId() << ","
                         << totalWallet.getBalance() << ","
                         << static_cast<int>(WalletType::TOTAL) << "\n";
            
            // Initialize admin wallet
            createWallets << "admin,0,0\n";
            createWallets.close();
        } else {
            // Load existing wallets
            std::string line;
            bool foundTotalWallet = false;
            int adminBalance = 0;
            
            while (std::getline(checkWallets, line)) {
                std::stringstream ss(line);
                std::string walletId;
                int balance, type;
                char comma;
                
                if (!(ss >> walletId >> comma >> balance >> comma >> type)) {
                    continue; // Bỏ qua dòng không hợp lệ
                }
                
                if (walletId.empty()) {
                    continue; // Bỏ qua dòng không hợp lệ
                }
                
                if (static_cast<WalletType>(type) == WalletType::TOTAL) {
                    if (!foundTotalWallet) {
                        // This is the total wallet
                        totalWallet = Wallet(walletId, balance, WalletType::TOTAL);
                        foundTotalWallet = true;
                    }
                } else if (static_cast<WalletType>(type) == WalletType::USER && walletId == "admin") {
                    // Lưu balance của admin
                    adminBalance = balance;
                }
            }
            
            // Nếu không tìm thấy ví tổng, tạo mới
            if (!foundTotalWallet) {
                totalWallet = Wallet(TOTAL_WALLET_ID, 0, WalletType::TOTAL);
            }
            
            // Cập nhật balance cho admin
            auto adminIt = users.find("admin");
            if (adminIt != users.end()) {
                Wallet& wallet = adminIt->second.getWallet();
                int currentBalance = wallet.getBalance();
                int diff = adminBalance - currentBalance;
                if (diff != 0) {
                    if (diff > 0) {
                        wallet.addPoints(diff);
                    } else {
                        wallet.deductPoints(-diff);
                    }
                }
            }
        }
        checkWallets.close();

        // Create transactions file if it doesn't exist
        std::string transactionsPath = dataDir + "/transactions.txt";
        std::ifstream checkTransactions(transactionsPath);
        if (!checkTransactions.good()) {
            std::ofstream createTransactions(transactionsPath);
            if (!createTransactions.is_open()) {
                std::cout << "Loi: Khong the tao file transactions.txt!\n";
                return;
            }
            createTransactions.close();
        }
        checkTransactions.close();
        
        // Lưu lại tất cả dữ liệu
        saveToFile(dataFile);
        
        isSystemInitialized = true;
    }
}

bool UserManager::addPointsToTotalWallet(int points, const User& user) {
    if (!isSystemInitialized || !user.getIsAdmin()) {
        return false;
    }
    totalWallet.addPoints(points);
    return true;
}

int UserManager::getTotalWalletBalance() const {
    return totalWallet.getBalance();
}

bool UserManager::viewTotalWalletInfo(const User& user) const {
    if (!user.getIsAdmin()) {
        return false;
    }
    
    std::cout << "Tong so diem: " << totalWallet.getBalance() << " diem\n";
    return true;
}

bool UserManager::createNewPoints(int amount, const User& user) {
    if (!isSystemInitialized || !user.getIsAdmin()) {
        return false;
    }

    // Thêm điểm vào ví tổng
    if (!addPointsToTotalWallet(amount, user)) {
        return false;
    }

    // Tạo mã giao dịch duy nhất
    std::string transactionId = "SYSTEM_" + TOTAL_WALLET_ID + "_" + std::to_string(std::time(nullptr));

    // Ghi log giao dịch
    Transaction transaction(
        transactionId,
        "SYSTEM",
        TOTAL_WALLET_ID,
        amount,
        std::time(nullptr),
        "POINTS_CREATION"
    );
    totalWallet.addTransaction(transaction);

    return true;
}

User* UserManager::getUser(const std::string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool UserManager::transferPointsFromTotal(const std::string& transactionId, int points, User& receiver) {
    if (!isSystemInitialized) {
        std::cout << "He thong chua duoc khoi tao!\n";
        return false;
    }

    if (points <= 0) {
        std::cout << "So diem khong hop le!\n";
        return false;
    }

    // Kiểm tra số dư ví tổng
    if (totalWallet.getBalance() < points) {
        std::cout << "So du vi tong khong du!\n";
        return false;
    }

    // Tạo giao dịch
    Transaction transaction(
        transactionId,
        TOTAL_WALLET_ID,
        receiver.getWallet().getWalletId(),
        points,
        std::time(nullptr),
        "Chuyen diem tu vi tong"
    );

    // Trừ điểm từ ví tổng
    totalWallet.deductPoints(points);
    totalWallet.addTransaction(transaction);

    // Cộng điểm vào ví người nhận
    receiver.getWallet().addPoints(points);
    receiver.getWallet().addTransaction(transaction);

    // Lưu thay đổi
    saveToFile(dataFile);
    return true;
} 