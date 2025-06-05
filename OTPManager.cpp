#include "OTPManager.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string OTPManager::generateOTP() {
    std::srand(std::time(nullptr));
    int otp = 100000 + std::rand() % 900000; // 6 digits
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << otp;
    return oss.str();
}

bool OTPManager::verifyOTP(const std::string& inputOTP, const std::string& actualOTP) {
    return inputOTP == actualOTP;
}
