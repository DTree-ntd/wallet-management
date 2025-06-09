#include "Wallet.h"
#include <iostream>
#include <iomanip>

// Định nghĩa ID cố định cho ví tổng
const std::string Wallet::TOTAL_WALLET_ID = "TOTAL_WALLET_001";

Wallet::Wallet(const std::string& id) {
    walletId = id;
    type = WalletType::USER;
    balance = 0;
}

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
    if (type == WalletType::TOTAL) {
        // Ví tổng không thể bị trừ điểm trực tiếp
        return false;
    }
    
    if (balance >= points) {
        balance -= points;
        return true;
    }
    return false;
}

void Wallet::addTransaction(const Transaction& transaction) {
    transactionHistory.push_back(transaction);
}

void Wallet::printTransactionHistory() const {
    std::cout << "Transaction History for Wallet " << walletId << ":\n";
    for (const auto& transaction : transactionHistory) {
        std::cout << "Transaction ID: " << transaction.transactionId << "\n"
                  << "From: " << transaction.fromWalletId << "\n"
                  << "To: " << transaction.toWalletId << "\n"
                  << "Points: " << transaction.points << "\n"
                  << "Time: " << std::ctime(&transaction.timestamp)
                  << "Status: " << transaction.status << "\n"
                  << "------------------------\n";
    }
}
