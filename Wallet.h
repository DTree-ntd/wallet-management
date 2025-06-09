#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>

enum class WalletType {
    TOTAL,    // Ví tổng
    USER      // Ví người dùng
};

struct Transaction {
    std::string transactionId;    
    std::string fromWalletId;
    std::string toWalletId;
    int points;                   
    std::time_t timestamp;       
    std::string status;           

    Transaction() = default;

    Transaction(std::string id, std::string from, std::string to, int pts, std::time_t time, std::string stat)
        : transactionId(id), fromWalletId(from), toWalletId(to), points(pts), timestamp(time), status(stat) {}
};

class Wallet {
private:
    std::string walletId;           
    mutable int balance;                   
    mutable std::vector<Transaction> transactionHistory; 
    WalletType type;

public:
    // Constructor mặc định
    Wallet() : balance(0), type(WalletType::USER) {}

    // Constructor đầy đủ
    Wallet(const std::string& id, int initialBalance, WalletType walletType)
        : walletId(id), balance(initialBalance), type(walletType) {}

    // Constructor cho ví người dùng
    Wallet(const std::string& id) : walletId(id), balance(0), type(WalletType::USER) {}

    // Kiểm tra xem có phải ví tổng không
    bool isTotalWallet() const { return type == WalletType::TOTAL; }

    const std::string& getWalletId() const { return walletId; }
    int getBalance() const { return balance; }
    WalletType getType() const { return type; }

    // Chỉ ví tổng mới có thể tăng điểm không giới hạn
    void addPoints(int points) const {
        balance += points;
    }

    bool deductPoints(int points) const {
        if (balance >= points) {
            balance -= points;
            return true;
        }
        return false;
    }

    void addTransaction(const Transaction& transaction) const {
        transactionHistory.push_back(transaction);
    }

    void printTransactionHistory() const {
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

    const std::vector<Transaction>& getTransactionHistory() const { return transactionHistory; }
};

#endif // WALLET_H
