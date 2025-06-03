#include <iostream>
#include <limits>
#include "UserManager.h"
#include "Wallet.h"
#include "TransactionManager.h"

void userMenu(User& currentUser) {
    while (true) {
        std::cout << "\n--- Menu nguoi dung: " << currentUser.getUsername() << " ---\n";
        std::cout << "1. Xem so du cac vi\n";
        std::cout << "2. Xem lich su giao dich\n";
        std::cout << "3. Chuyen diem giua cac vi\n";
        std::cout << "4. Dang xuat\n";
        std::cout << "Chon chuc nang (1-4): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::cout << "\nSo du cac vi:\n";
            for (const auto& w : currentUser.getWallets()) {
                std::cout << " - Vi ID: " << w.getWalletId() << ", So diem: " << w.getBalance() << "\n";
            }
        }
        else if (choice == 2) {
            std::cout << "\nNhap ID vi can xem lich su: ";
            std::string walletId;
            std::getline(std::cin, walletId);

            Wallet* wallet = currentUser.findWalletById(walletId);
            if (wallet) {
                wallet->printTransactionHistory();
            } else {
                std::cout << "Vi khong ton tai.\n";
            }
        }
        else if (choice == 3) {
            std::string fromWalletId, toWalletId;
            int points;

            std::cout << "Nhap ID vi chuyen diem: ";
            std::getline(std::cin, fromWalletId);
            std::cout << "Nhap ID vi nhan diem: ";
            std::getline(std::cin, toWalletId);
            std::cout << "Nhap so diem can chuyen: ";
            std::cin >> points;
            std::cin.ignore();

            // Tạm thời giả định người nhận cũng là currentUser
            bool success = TransactionManager::transferPoints(currentUser, fromWalletId, currentUser, toWalletId, points);
            if (!success) {
                std::cout << "Giao dich that bai.\n";
            }
        }
        else if (choice == 4) {
            std::cout << "Dang xuat...\n";
            break;
        }
        else {
            std::cout << "Lua chon khong hop le.\n";
        }
    }
}

int main() {
    UserManager userManager;

    while (true) {
        std::cout << "\n==== HE THONG QUAN LY TAI KHOAN ====\n";
        std::cout << "1. Dang ky\n";
        std::cout << "2. Dang nhap\n";
        std::cout << "3. Thoat\n";
        std::cout << "Chon chuc nang (1-3): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string username, password, fullName, email, phoneNumber;

            std::cout << "Nhap ten dang nhap: ";
            std::getline(std::cin, username);
            std::cout << "Nhap mat khau: ";
            std::getline(std::cin, password);
            std::cout << "Nhap ho ten: ";
            std::getline(std::cin, fullName);
            std::cout << "Nhap email: ";
            std::getline(std::cin, email);
            std::cout << "Nhap so dien thoai: ";
            std::getline(std::cin, phoneNumber);

            if (userManager.registerUser(username, password, fullName, email, phoneNumber)) {
                std::cout << "Dang ky thanh cong!\n";

                // Tạo ví mặc định cho user sau khi đăng ký
                User* newUser = userManager.getUserByUsername(username);
                if (newUser) {
                    std::string defaultWalletId = username + "_wallet1";
                    newUser->addWallet(Wallet(defaultWalletId));
                    std::cout << "Vi mac dinh da duoc tao: " << defaultWalletId << std::endl;
                }
            } else {
                std::cout << "Dang ky that bai. Ten dang nhap da ton tai.\n";
            }
        }
        else if (choice == 2) {
            std::string username, password;

            std::cout << "Nhap ten dang nhap: ";
            std::getline(std::cin, username);
            std::cout << "Nhap mat khau: ";
            std::getline(std::cin, password);

            if (userManager.loginUser(username, password)) {
                std::cout << "Dang nhap thanh cong!\n";
                User* currentUser = userManager.getUserByUsername(username);
                if (currentUser) {
                    userMenu(*currentUser);
                }
            } else {
                std::cout << "Dang nhap that bai. Kiem tra lai ten dang nhap hoac mat khau.\n";
            }
        }
        else if (choice == 3) {
            std::cout << "Tam biet!\n";
            break;
        }
        else {
            std::cout << "Lua chon khong hop le.\n";
        }
    }

    return 0;
}
