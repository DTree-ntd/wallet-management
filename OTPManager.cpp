#include "OTPManager.h"

std::string OTPManager::generateOTP(const std::string& username) {
    std::string otp;
    for (int i = 0; i < 6; ++i) {
        otp += std::to_string(std::rand() % 10);
    }

    time_t now = std::time(nullptr);
    otpStore[username] = {otp, now};
    return otp;
}

bool OTPManager::verifyOTP(const std::string& username, const std::string& otpCode) {
    if (otpStore.find(username) == otpStore.end()) return false;

    auto& [storedOTP, timestamp] = otpStore[username];
    time_t now = std::time(nullptr);
    if (now - timestamp > expirySeconds) {
        otpStore.erase(username);
        return false;
    }

    if (storedOTP == otpCode) {
        otpStore.erase(username);
        return true;
    }

    return false;
}
