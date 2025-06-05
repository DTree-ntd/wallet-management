#ifndef USER_H
#define USER_H

#include "OTPManager.h"
#include <chrono>
#pragma once
#include <string>
#include <unordered_map>
#include <ctime>

struct OTPInfo {
    std::string otpCode;
    std::chrono::system_clock::time_point expiry;
};

    std::unordered_map<std::string, OTPInfo> otpStore;
    const int expirySeconds = 300; // OTP có hiệu lực 5 phút

public:
    std::string generateOTP(const std::string& username);
    bool verifyOTP(const std::string& username, const std::string& otpCode);
};

#endif
