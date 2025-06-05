#include "OTPManager.h"
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>

std::string OTPManager::generateOTP(const std::string& username) {
    int otp = rand() % 900000 + 100000;
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << otp;
    std::string otpCode = oss.str();

    otpStore[username] = OTPInfo{otpCode, std::time(nullptr)};
    return otpCode;
}

bool OTPManager::verifyOTP(const std::string& username, const std::string& otpCode) {
    if (otpStore.find(username) == otpStore.end()) {
        return false;
    }

    OTPInfo info = otpStore[username]; // Không dùng structured binding

    std::time_t now = std::time(nullptr);
    if (now - info.generatedTime > expirySeconds) {
        otpStore.erase(username);
        return false;
    }

    if (info.otp == otpCode) {
        otpStore.erase(username); // Xóa sau khi dùng
        return true;
    }

    return false;
}
