#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>

struct Transaction {
    std::string transactionId;    // 
    std::string fromWalletId;
    std::string toWalletId;
    int points;                   // 
    std::time_t timestamp;       // 
    std::string status;           // 

    Transaction() = default;

    Transaction(std::string id, std::string from, std::string to, int pts, std::time_t time, std::string stat)
        : transactionId(id), fromWalletId(from), toWalletId(to), points(pts), timestamp(time), status(stat) {}
};

class Wallet {
private:
    std::string walletId;           
    int balance;                   
    std::vector<Transaction> transactionHistory; 

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









