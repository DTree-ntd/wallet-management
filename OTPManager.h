#pragma once
#include <string>
#include <map>
#include <ctime>
#include <qrencode.h>

class OTPManager {
private:
    std::string secretKey;
    std::map<std::string, std::pair<std::string, time_t>> tempOTPs; // username -> (secretKey, expiryTime)

public:
    OTPManager();
    ~OTPManager();

    // Temporary OTP methods
    std::string generateTempOTP(const std::string& username);
    bool verifyTempOTP(const std::string& username, const std::string& otp);
    void removeTempOTP(const std::string& username);
    std::string getCurrentOTP();
    
    // QR code generation
    void printQRCode(const std::string& otp);

    // Getter for secretKey
    const std::string& getSecretKey() const { return secretKey; }
}; 