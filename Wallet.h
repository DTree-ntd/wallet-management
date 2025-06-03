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









