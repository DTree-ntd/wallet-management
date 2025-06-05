#include "UserManager.h"
#include <iostream>
#include <string>
#include <limits>
#include "OTPManager.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showMenu() {
    clearScreen();
    std::cout << "\n=== HE THONG QUAN LY VI DIEM THUONG ===\n";
    std::cout << "1. Dang ky tai khoan\n";
    std::cout << "2. Dang nhap\n";
    std::cout << "3. Thoat\n";
    std::cout << "Chon chuc nang (1-3): ";
}

void registerUser(UserManager& userManager) {
    clearScreen();
    std::string username, password, fullName, email, phoneNumber;
    
    std::cout << "\n=== DANG KY TAI KHOAN ===\n";
    
    std::cout << "Nhap ten dang nhap: ";
    std::cin >> username;
    
    std::cout << "Nhap mat khau: ";
    std::cin >> password;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Nhap ho va ten: ";
    std::getline(std::cin, fullName);
    
    std::cout << "Nhap email: ";
    std::cin >> email;
    
    std::cout << "Nhap so dien thoai: ";
    std::cin >> phoneNumber;
    
    if (userManager.registerUser(username, password, fullName, email, phoneNumber)) {
        std::cout << "Dang ky thanh cong!\n";
    } else {
        std::cout << "Dang ky that bai! Vui long kiem tra lai thong tin.\n";
    }
}

void login(UserManager& userManager, OTPManager& otpManager) {
    clearScreen();
    std::string username, password;
    
    std::cout << "\n=== DANG NHAP ===\n";
    std::cout << "Nhap ten dang nhap: ";
    std::cin >> username;
    std::cout << "Nhap mat khau: ";
    std::cin >> password;
    
    if (userManager.login(username, password)) {
        std::string otp = otpManager.generateOTP(username);
        std::cout << "Ma OTP cua ban la: " << otp << "\n"; // Thay vì gửi email, in ra console
        std::cout << "Nhap ma OTP: ";
        std::string inputOtp;
        std::cin >> inputOtp;

        if (otpManager.verifyOTP(username, inputOtp)) {
            std::cout << "Dang nhap thanh cong (da xac thuc OTP)!\n";
            // TODO: Menu nguoi dung dang nhap thanh cong
        } else {
            std::cout << "OTP khong hop le hoac het han.\n";
        }
    } else {
        std::cout << "Dang nhap that bai! Vui long kiem tra lai thong tin.\n";
    }
}

int main() {
    UserManager userManager("../data/users.txt");
    OTPManager otpManager;
    int choice;

    while (true) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                registerUser(userManager);
                break;
            case 2:
                login(userManager, otpManager);
                break;
            case 3:
                clearScreen();
                std::cout << "Cam on ban da su dung chuong trinh!\n";
                return 0;
            default:
                std::cout << "Lua chon khong hop le! Vui long chon lai.\n";
        }

        std::cout << "\nNhan Enter de tiep tuc...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    return 0;
}
