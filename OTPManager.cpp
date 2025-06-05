#include "OTPManager.h"
#include <random>
#include <sstream>
#include <iomanip>

std::string OTPManager::generateOTP(const std::string& username) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 999999);

    int otpValue = dist(gen);
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << otpValue;

    std::string otpCode = oss.str();
    otpStore[username] = {otpCode, std::chrono::steady_clock::now()};

    // Thay vì gửi email, in ra console để demo
    std::cout << "[OTP] Ma OTP cua ban la: " << otpCode << std::endl;
    return otpCode;
}

bool OTPManager::verifyOTP(const std::string& username, const std::string& otpCode) {
    if (otpStore.find(username) == otpStore.end()) return false;

    auto stored = otpStore[username];
    auto storedOTP = stored.first;
    auto timestamp = stored.second;

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - timestamp).count() > expirySeconds) {
        otpStore.erase(username);
        return false;
    }

    if (storedOTP == otpCode) {
        otpStore.erase(username);
        return true;
    }

    return false;
}
