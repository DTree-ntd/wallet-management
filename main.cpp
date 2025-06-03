#include <iostream>
#include "UserManager.h"
#include "wallet.h"
#include <string>
#include <limits>

using namespace std;

int main() {
    UserManager userManager;
    Wallet Wallet;

    string currentUsername = "";
    int choice;

    while (true) {
        if (currentUsername.empty()) {
            // Menu chính khi chưa đăng nhập
            cout << "\n===== MENU CHINH =====\n";
            cout << "1. Dang ky\n";
            cout << "2. Dang nhap\n";
            cout << "3. Thoat\n";
            cout << "Lua chon: ";
            cin >> choice;

            if (choice == 1) {
                userManager.registerUser();
            } else if (choice == 2) {
                string username;
                if (userManager.loginUser(username)) {
                    currentUsername = username;
                    Wallet.createWalletIfNotExist(currentUsername);
                    cout << "Dang nhap thanh cong!\n";
                }
            } else if (choice == 3) {
                cout << "Tam biet!\n";
                break;
            } else {
                cout << "Lua chon khong hop le.\n";
            }
        } else {
            // Menu sau khi đăng nhập
            cout << "\n===== MENU NGUOI DUNG =====\n";
            cout << "1. Xem so du vi\n";
            cout << "2. Chuyen diem\n";
            cout << "3. Dang xuat\n";
            cout << "Lua chon: ";
            cin >> choice;

            if (choice == 1) {
                Wallet.viewBalance(currentUsername);
            } else if (choice == 2) {
                string receiverUsername;
                double amount;

                cout << "Nhap ten nguoi nhan: ";
                cin >> receiverUsername;
                cout << "Nhap so diem muon chuyen: ";
                cin >> amount;

                Wallet.transferPoints(currentUsername, receiverUsername, amount);
            } else if (choice == 3) {
                cout << "Dang xuat thanh cong.\n";
                currentUsername = ""; // Đăng xuất
            } else {
                cout << "Lua chon khong hop le.\n";
            }
        }
    }

    return 0;
}
