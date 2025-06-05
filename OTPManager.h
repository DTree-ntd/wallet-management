#pragma once
#include <string>

class OTPManager {
public:
    static std::string generateOTP();
    static bool verifyOTP(const std::string& inputOTP, const std::string& actualOTP);
};
