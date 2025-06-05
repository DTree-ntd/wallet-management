#pragma once
#include <string>
#include <unordered_map>
#include <ctime>

class OTPManager {
private:
    struct OTPInfo {
        std::string otp;
        std::time_t generatedTime;
    };

    std::unordered_map<std::string, OTPInfo> otpStore;
    const int expirySeconds = 300; // OTP có hiệu lực 5 phút

public:
    std::string generateOTP(const std::string& username);
    bool verifyOTP(const std::string& username, const std::string& otpCode);
};
