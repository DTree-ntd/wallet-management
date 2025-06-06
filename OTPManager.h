#pragma once
#include <string>
extern "C" {
    #include "lib/cotp/cotp.h"
}
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <ctime>

// Forward declarations
COTPRESULT totp_algo(const char* input, size_t input_len, char* output, size_t* output_len);
uint64_t totp_time();

class OTPManager {
private:
    std::string secretKey;
    OTPData* otpData;

public:
    OTPManager();
    ~OTPManager();

    // Tao secret key moi cho user
    std::string generateSecretKey();

    // Tao QR code URI de user co the quet ma
    std::string generateQRCodeURI(const std::string& username);

    // Xac thuc ma OTP
    bool verifyOTP(const std::string& otp);

    // Lay ma OTP hien tai
    std::string getCurrentOTP();

    // Luu secret key vao file
    bool saveSecretKey(const std::string& username, const std::string& secretKey);

    // Doc secret key tu file
    std::string loadSecretKey(const std::string& username);

    // Getter cho secretKey
    const std::string& getSecretKey() const { return secretKey; }
}; 