#include "UserManager.h"
#include "OTPManager.h"
#include "WalletManager.h"
#include <iostream>
#include <string>
#include <limits>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showMainMenu() {
    std::cout << "\n=== HE THONG QUAN LY VI DIEM THUONG ===\n";
    std::cout << "1. Dang ky tai khoan\n";
    std::cout << "2. Dang nhap\n";
    std::cout << "3. Thoat\n";
    std::cout << "Chon chuc nang (1-3): ";
}

void showUserMenu() {
    std::cout << "\n=== MENU NGUOI DUNG ===\n";
    std::cout << "1. Tao vi\n";
    std::cout << "2. Xem so du\n";
    std::cout << "3. Lich su giao dich\n";
    std::cout << "4. Chuyen diem\n";
    std::cout << "5. Dang xuat\n";
    std::cout << "Chon chuc nang (1-5): ";
}

void userSession(const std::string& username, WalletManager& walletManager, UserManager& userManager) {
    int choice;
    std::string otp = OTPManager::generateOTP();
    std::string userOtp;

    std::cout << "Ma OTP cua ban la: " << otp << "\nNhap OTP de tiep tuc: ";
    std::cin >> userOtp;

    if (!OTPManager::verifyOTP(userOtp, otp)) {
        std::cout << "OTP sai! Quay lai menu chinh.\n";
        return;
    }

    while (true) {
        clearScreen();
        showUserMenu();
        std::cin >> choice;

        if (choice == 1) {
            std::string walletId = walletManager.createWallet();
            userManager.addWalletToUser(username, walletId);
            std::cout << "Da tao vi: " << walletId << "\n";
        } else if (choice == 2) {
            for (const auto& id : userManager.getUser(username).getWalletIds()) {
                int balance = walletManager.getBalance(id);
                std::cout << "Vi " << id << ": " << balance << " diem\n";
            }
        } else if (choice == 3) {
            for (const auto& id : userManager.getUser(username).getWalletIds()) {
                auto history = walletManager.getTransactionHistory(id);
                std::cout << "Giao dich cua vi " << id << ":\n";
                for (const auto& tx : history) {
                    std::cout << "Tu: " << tx.fromWalletId << " -> " << tx.toWalletId
                              << " | Diem: " << tx.points << " | Thoi gian: " << tx.timestamp << "\n";
                }
            }
        } else if (choice == 4) {
            std::string fromId, toId;
            int amount;
            std::cout << "Nhap vi nguon: "; std::cin >> fromId;
            std::cout << "Nhap vi dich: "; std::cin >> toId;
            std::cout << "Nhap so diem muon chuyen: "; std::cin >> amount;

            if (walletManager.transferPoints(fromId, toId, amount))
                std::cout << "Chuyen thanh cong!\n";
            else
                std::cout << "Chuyen that bai. Kiem tra lai.\n";
        } else if (choice == 5) {
            std::cout << "Dang xuat...\n"; break;
        } else {
            std::cout << "Lua chon khong hop le!\n";
        }

        std::cout << "Nhan Enter de tiep tuc...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

int main() {
    UserManager userManager("../data/users.txt");
    WalletManager walletManager("../data/wallets.txt");
    int choice;

    while (true) {
        clearScreen();
        showMainMenu();
        std::cin >> choice;

        if (choice == 1) {
            registerUser(userManager);
        } else if (choice == 2) {
            std::string username, password;
            std::cout << "Ten dang nhap: "; std::cin >> username;
            std::cout << "Mat khau: "; std::cin >> password;

            if (userManager.login(username, password)) {
                userSession(username, walletManager, userManager);
            } else {
                std::cout << "Dang nhap that bai.\n";
            }
        } else if (choice == 3) {
            std::cout << "Tam biet!\n"; break;
        } else {
            std::cout << "Khong hop le!\n";
        }

        std::cout << "Nhan Enter de tiep tuc...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    return 0;
}
