#pragma once
#include <string>
#include <map>
#include "Wallet.h"

class WalletManager {
private:
    std::map<std::string, Wallet> wallets;
    std::string dataPath;

    std::string getCurrentTimestamp();
    void loadWallets();
    void saveWallets();

public:
    WalletManager(const std::string& path);

    std::string createWallet();
    bool transferPoints(const std::string& fromId, const std::string& toId, int points);
    int getBalance(const std::string& walletId);
    std::vector<Transaction> getTransactionHistory(const std::string& walletId);

    bool walletExists(const std::string& walletId);
    Wallet& getWallet(const std::string& id);
};
