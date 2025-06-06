#include "PasswordHasher.h"
#include <stdexcept>

std::string PasswordHasher::generateSalt() {
    unsigned char salt[16];
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        throw std::runtime_error("Failed to generate random salt");
    }
    return toHex(salt, sizeof(salt));
}

std::string PasswordHasher::toHex(const unsigned char* data, size_t len) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; i++) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

std::vector<unsigned char> PasswordHasher::fromHex(const std::string& hex) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

std::string PasswordHasher::hashPassword(const std::string& password) {
    // Generate a random salt
    std::string salt = generateSalt();
    
    // Initialize the hash context
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create hash context");
    }
    
    // Initialize the hash operation
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize hash operation");
    }
    
    // Add the salt to the hash
    if (EVP_DigestUpdate(ctx, salt.c_str(), salt.length()) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to update hash with salt");
    }
    
    // Add the password to the hash
    if (EVP_DigestUpdate(ctx, password.c_str(), password.length()) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to update hash with password");
    }
    
    // Get the hash value
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    if (EVP_DigestFinal_ex(ctx, hash, &hashLen) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize hash");
    }
    
    // Clean up
    EVP_MD_CTX_free(ctx);
    
    // Combine salt and hash
    return salt + ":" + toHex(hash, hashLen);
}

bool PasswordHasher::verifyPassword(const std::string& password, const std::string& hashedPassword) {
    // Split the hashed password into salt and hash
    size_t colonPos = hashedPassword.find(':');
    if (colonPos == std::string::npos) {
        return false;
    }
    
    std::string salt = hashedPassword.substr(0, colonPos);
    std::string storedHash = hashedPassword.substr(colonPos + 1);
    
    // Initialize the hash context
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        return false;
    }
    
    // Initialize the hash operation
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        return false;
    }
    
    // Add the salt to the hash
    if (EVP_DigestUpdate(ctx, salt.c_str(), salt.length()) != 1) {
        EVP_MD_CTX_free(ctx);
        return false;
    }
    
    // Add the password to the hash
    if (EVP_DigestUpdate(ctx, password.c_str(), password.length()) != 1) {
        EVP_MD_CTX_free(ctx);
        return false;
    }
    
    // Get the hash value
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    if (EVP_DigestFinal_ex(ctx, hash, &hashLen) != 1) {
        EVP_MD_CTX_free(ctx);
        return false;
    }
    
    // Clean up
    EVP_MD_CTX_free(ctx);
    
    // Compare the computed hash with the stored hash
    return toHex(hash, hashLen) == storedHash;
} 