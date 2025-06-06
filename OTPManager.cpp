#include "OTPManager.h"
extern "C" {
    #include "cotp.h"
    #include "otpuri.h"
}
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <openssl/rand.h>
#include <cstring>

// Declare C functions
extern "C" {
    COTPRESULT otpuri_build_uri(OTPData* data, const char* issuer, const char* name, const char* digest, char* output);
    COTPRESULT totp_now(OTPData* data, char* out_str);
}

// Global variable for callback functions
static OTPManager* currentOTPManager = nullptr;

// Callback function for TOTP algorithm
COTPRESULT totp_algo(const char* input, size_t input_len, char* output, size_t* output_len) {
    if (!currentOTPManager) return -1;
    
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    const std::string& secretKey = currentOTPManager->getSecretKey();
    HMAC(EVP_sha1(), secretKey.c_str(), secretKey.length(),
         (unsigned char*)input, input_len, hash, &hash_len);
    
    memcpy(output, hash, hash_len);
    *output_len = hash_len;
    return 0;
}

// Callback function for time
uint64_t totp_time() {
    return time(NULL);
}

OTPManager::OTPManager() {
    otpData = (OTPData*)malloc(sizeof(OTPData));
    if (otpData) {
        otpData->algo = (COTP_ALGO)totp_algo;
        otpData->time = totp_time;
        otpData->digits = 6;
        otpData->interval = 30;
    }
    currentOTPManager = this;
}

OTPManager::~OTPManager() {
    if (otpData) {
        free(otpData);
    }
    if (currentOTPManager == this) {
        currentOTPManager = nullptr;
    }
}

std::string OTPManager::generateSecretKey() {
    unsigned char randomBytes[20];
    RAND_bytes(randomBytes, sizeof(randomBytes));
    
    std::stringstream ss;
    for(int i = 0; i < sizeof(randomBytes); i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)randomBytes[i];
    }
    
    secretKey = ss.str();
    return secretKey;
}

std::string OTPManager::generateQRCodeURI(const std::string& username) {
    char uri[256];
    OTPData otpData;
    otpData.method = TOTP;
    otpData.digits = 6;
    otpData.interval = 30;
    otpData.base32_secret = secretKey.c_str();

    if (otpuri_build_uri(&otpData, username.c_str(), username.c_str(), "SHA1", uri) == OTP_OK) {
        return std::string(uri);
    }
    return "";
}

bool OTPManager::verifyOTP(const std::string& otp) {
    if (secretKey.empty()) {
        return false;
    }
    
    char currentOTP[7];
    if (totp_now(otpData, currentOTP) == 0) {
        return (otp == currentOTP);
    }
    return false;
}

std::string OTPManager::getCurrentOTP() {
    if (secretKey.empty()) {
        return "";
    }
    
    char otp[7];
    if (totp_now(otpData, otp) == 0) {
        return std::string(otp);
    }
    return "";
}

bool OTPManager::saveSecretKey(const std::string& username, const std::string& secretKey) {
    std::ofstream file("data/otp_keys.txt", std::ios::app);
    if (!file.is_open()) {
        return false;
    }
    
    file << username << "|" << secretKey << "\n";
    file.close();
    return true;
}

std::string OTPManager::loadSecretKey(const std::string& username) {
    std::ifstream file("data/otp_keys.txt");
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string storedUsername, storedKey;
        std::getline(ss, storedUsername, '|');
        std::getline(ss, storedKey, '|');
        
        if (storedUsername == username) {
            return storedKey;
        }
    }
    
    return "";
} 