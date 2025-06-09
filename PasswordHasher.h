#pragma once
#include <string>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>

class PasswordHasher {
public:
    // Hash a password with a random salt
    static std::string hashPassword(const std::string& password);
    
    // Verify a password against a hashed password
    static bool verifyPassword(const std::string& password, const std::string& hashedPassword);

private:
    // Generate a random salt
    static std::string generateSalt();
    
    // Convert binary data to hex string
    static std::string toHex(const unsigned char* data, size_t len);
    
    // Convert hex string to binary data
    static std::vector<unsigned char> fromHex(const std::string& hex);
}; 