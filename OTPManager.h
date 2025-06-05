#pragma once
#include <string>
#include <unordered_map>
#include <ctime>
#include <cstdlib>

class OTPManager {
private:
    std::unordered_map<std::string, std::pair<std::string, time_t>> otpStore;
    int expirySeconds = 300; // 5 phút

public:
    OTPManager() {
        std::srand(std::time(nullptr));
    }

    std::string generateOTP(const std::string& username);
    bool verifyOTP(const std::string& username, const std::string& otpCode);
};
