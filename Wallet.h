#pragma once
#include <string>
#include <vector>

struct Transaction {
    std::string fromWalletId;
    std::string toWalletId;
    int points;
    std::string timestamp;
};

class Wallet {
private:
    std::string walletId;
    int balance;
    std::vector<Transaction> history;

public:
    Wallet();
    Wallet(const std::string& id, int initialBalance = 0);

    std::string getId() const;
    int getBalance() const;
    const std::vector<Transaction>& getHistory() const;

    void addPoints(int amount);
    bool deductPoints(int amount);
    void addTransaction(const Transaction& tx);
};
