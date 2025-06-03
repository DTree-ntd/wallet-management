#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>

struct Transaction {
    std::string transactionId;    // Mã định danh giao dịch duy nhất
    std::string fromWalletId;
    std::string toWalletId;
    int points;                   // Số điểm chuyển
    std::time_t timestamp;       // Thời gian giao dịch
    std::string status;           // "Success" hoặc "Failed"

    Transaction() = default;

    Transaction(std::string id, std::string from, std::string to, int pts, std::time_t time, std::string stat)
        : transactionId(id), fromWalletId(from), toWalletId(to), points(pts), timestamp(time), status(stat) {}
};

class Wallet {
private:
    std::string walletId;           // Mã định danh ví duy nhất
    int balance;                   // Số điểm hiện tại
    std::vector<Transaction> transactionHistory; // Lịch sử giao dịch liên quan

public:
    Wallet(const std::string& id);

    const std::string& getWalletId() const;

    int getBalance() const;

    void addPoints(int points);

    bool deductPoints(int points);

    void addTransaction(const Transaction& transaction);

    void printTransactionHistory() const;
};

#endif // WALLET_H
2. Tạo file Wallet.cpp
cpp
Sao chép
Chỉnh sửa
#include "Wallet.h"
#include <iostream>
#include <iomanip>

Wallet::Wallet(const std::string& id) : walletId(id), balance(0) {}

const std::string& Wallet::getWalletId() const {
    return walletId;
}

int Wallet::getBalance() const {
    return balance;
}

void Wallet::addPoints(int points) {
    balance += points;
}

bool Wallet::deductPoints(int points) {
    if (points > balance) {
        return false;
    }
    balance -= points;
    return true;
}

void Wallet::addTransaction(const Transaction& transaction) {
    transactionHistory.push_back(transaction);
}

void Wallet::printTransactionHistory() const {
    std::cout << "Lich su giao dich cho vi: " << walletId << std::endl;
    std::cout << std::left << std::setw(15) << "ID Giao dich"
              << std::setw(15) << "Tu Vi"
              << std::setw(15) << "Den Vi"
              << std::setw(10) << "Diem"
              << std::setw(25) << "Thoi gian"
              << std::setw(10) << "Trang thai" << std::endl;

    for (const auto& t : transactionHistory) {
        std::tm* tm_ptr = std::localtime(&t.timestamp);
        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_ptr);

        std::cout << std::setw(15) << t.transactionId
                  << std::setw(15) << t.fromWalletId
                  << std::setw(15) << t.toWalletId
                  << std::setw(10) << t.points
                  << std::setw(25) << timeStr
                  << std::setw(10) << t.status
                  << std::endl;
    }
}
3. Bổ sung trong User.h và User.cpp
Bạn có thể mở rộng lớp User để quản lý danh sách các ví (Wallet) của người dùng.

Trong User.h thêm đoạn này (trước public:):
cpp
Sao chép
Chỉnh sửa
#include "Wallet.h"
#include <vector>
Thêm thuộc tính private:

cpp
Sao chép
Chỉnh sửa
private:
    std::vector<Wallet> wallets;
Thêm các phương thức public:

cpp
Sao chép
Chỉnh sửa
public:
    // Thêm ví mới cho user, với walletId tự tạo hoặc truyền vào
    void addWallet(const Wallet& wallet);

    // Lấy danh sách ví
    std::vector<Wallet>& getWallets();

    // Tìm ví theo walletId, trả về con trỏ nullptr nếu không tìm thấy
    Wallet* findWalletById(const std::string& walletId);
Trong User.cpp thêm phần định nghĩa:
cpp
Sao chép
Chỉnh sửa
void User::addWallet(const Wallet& wallet) {
    wallets.push_back(wallet);
}

std::vector<Wallet>& User::getWallets() {
    return wallets;
}

Wallet* User::findWalletById(const std::string& walletId) {
    for (auto& w : wallets) {
        if (w.getWalletId() == walletId) {
            return &w;
        }
    }
    return nullptr;
}
4. Tạo lớp mới TransactionManager để xử lý giao dịch chuyển điểm giữa các ví, đảm bảo atomic và ghi log
Tạo file TransactionManager.h
cpp
Sao chép
Chỉnh sửa
#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <string>
#include "User.h"
#include <ctime>

class TransactionManager {
public:
    // Thực hiện chuyển điểm giữa 2 ví thuộc 2 user khác nhau hoặc cùng user
    // Trả về true nếu thành công, false nếu lỗi (ví dụ ví không đủ điểm)
    static bool transferPoints(User& fromUser, const std::string& fromWalletId,
                               User& toUser, const std::string& toWalletId,
                               int points);

    // Tạo mã giao dịch duy nhất (có thể đơn giản là timestamp + ngẫu nhiên)
    static std::string generateTransactionId();
};

#endif // TRANSACTIONMANAGER_H
Tạo file TransactionManager.cpp
cpp
Sao chép
Chỉnh sửa
#include "TransactionManager.h"
#include <random>
#include <sstream>
#include <iostream>

bool TransactionManager::transferPoints(User& fromUser, const std::string& fromWalletId,
                                        User& toUser, const std::string& toWalletId,
                                        int points) {
    Wallet* fromWallet = fromUser.findWalletById(fromWalletId);
    Wallet* toWallet = toUser.findWalletById(toWalletId);

    if (!fromWallet) {
        std::cout << "Vi nguon khong ton tai.\n";
        return false;
    }
    if (!toWallet) {
        std::cout << "Vi dich khong ton tai.\n";
        return false;
    }
    if (fromWallet->getBalance() < points) {
        std::cout << "So du vi khong du. Khong the chuyen.\n";
        return false;
    }

    // Bắt đầu giao dịch (atomic)
    bool deducted = fromWallet->deductPoints(points);
    if (!deducted) {
        std::cout << "Tru diem that bai.\n";
        return false;
    }

    toWallet->addPoints(points);

    // Ghi lại giao dịch với trạng thái success
    std::string transactionId = generateTransactionId();
    std::time_t now = std::time(nullptr);
    Transaction transaction(transactionId, fromWalletId, toWalletId, points, now, "Success");

    fromWallet->addTransaction(transaction);
    toWallet->addTransaction(transaction);

    std::cout << "Chuyen diem thanh cong! Ma giao dich: " << transactionId << std::endl;

    return true;
}

std::string TransactionManager::generateTransactionId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);

    std::stringstream ss;
    ss << std::time(nullptr) << dis(gen);
    return ss.str();
}
5. Bổ sung phần menu và thao tác chuyển điểm, xem ví, xem lịch sử trong main.cpp
5.1 Thêm các #include mới ở đầu file main.cpp:
cpp
Sao chép
Chỉnh sửa
#include "Wallet.h"
#include "TransactionManager.h"
#include <vector>
5.2 Thêm hàm phụ để hiện menu người dùng sau đăng nhập:
cpp
Sao chép
Chỉnh sửa
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

            std::cout << "Nhap ID vi chuyen diem









