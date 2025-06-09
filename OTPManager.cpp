#include "OTPManager.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>

OTPManager::OTPManager() {
}

OTPManager::~OTPManager() {
}

void OTPManager::printQRCode(const std::string& otp) {
    // Generate QR code
    QRcode* qr = QRcode_encodeString(otp.c_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qr) {
        std::cerr << "Error: Could not generate QR code\n";
        return;
    }

    // Print top border
    std::cout << "┌";
    for (int i = 0; i < qr->width * 2; i++) {
        std::cout << "─";
    }
    std::cout << "┐\n";

    // Print QR code content
    for (int y = 0; y < qr->width; y++) {
        std::cout << "│";
        for (int x = 0; x < qr->width; x++) {
            // Check if the module is set (black)
            if (qr->data[y * qr->width + x] & 1) {
                std::cout << "██";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << "│\n";
    }

    // Print bottom border
    std::cout << "└";
    for (int i = 0; i < qr->width * 2; i++) {
        std::cout << "─";
    }
    std::cout << "┘\n";

    // Free QR code
    QRcode_free(qr);
}

std::string OTPManager::generateTempOTP(const std::string& username) {
    // Generate a random 6-digit OTP
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);
    int otp = dis(gen);
    
    // Convert to string with leading zeros
    std::stringstream ss;
    ss << std::setw(6) << std::setfill('0') << otp;
    std::string otpStr = ss.str();
    
    // Store with 5 minutes expiry
    time_t expiryTime = time(nullptr) + 300; // 5 minutes
    tempOTPs[username] = std::make_pair(otpStr, expiryTime);
    
    return otpStr;
}

bool OTPManager::verifyTempOTP(const std::string& username, const std::string& otp) {
    auto it = tempOTPs.find(username);
    if (it == tempOTPs.end()) {
        return false;
    }
    
    // Check if OTP has expired
    if (time(nullptr) > it->second.second) {
        tempOTPs.erase(it);
        return false;
    }
    
    // Verify OTP
    bool result = (otp == it->second.first);
    
    // Remove OTP after verification
    tempOTPs.erase(it);
    
    return result;
}

void OTPManager::removeTempOTP(const std::string& username) {
    tempOTPs.erase(username);
}

std::string OTPManager::getCurrentOTP() {
    // Find the first non-expired OTP
    for (auto it = tempOTPs.begin(); it != tempOTPs.end(); ++it) {
        if (time(nullptr) <= it->second.second) {
            return it->second.first;
        }
    }
    return "";
} 