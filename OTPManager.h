#ifndef OTP_MANAGER_H
#define OTP_MANAGER_H

#include <string>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <iostream>

class OTPManager {
private:
    std::unordered_map<std::string, std::pair<std::string, time_t>> otpStore;
    int expirySeconds = 120; // OTP hết hạn sau 2 phút

public:
    OTPManager() {
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    std::string generateOTP(const std::string& username);
    bool verifyOTP(const std::string& username, const std::string& otpCode);
};

#endif
