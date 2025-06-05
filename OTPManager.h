#ifndef OTP_MANAGER_H
#define OTP_MANAGER_H

#pragma once
#include <string>
#include <map>
#include <chrono>

class OTPManager {
private:
    std::map<std::string, std::pair<std::string, std::chrono::steady_clock::time_point>> otpStore;
    const int expirySeconds = 300; // OTP có hiệu lực trong 5 phút

public:
    std::string generateOTP(const std::string& username);
    bool verifyOTP(const std::string& username, const std::string& otpCode);
};
