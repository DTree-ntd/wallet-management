#include "WalletManager.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <iostream>

WalletManager::WalletManager(const std::string& path) : dataPath(path) {
    loadWallets();
}

std::string WalletManager::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void WalletManager::loadWallets() {
    std::ifstream file(dataPath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id;
        int balance;
        std::getline(ss, id, '|');
        ss >> balance;
        Wallet wallet(id, balance);
        wallets[id] = wallet;
    }
    file.close();
}

void WalletManager::saveWallets() {
    std::ofstream file(dataPath);
    for (const auto& [id, wallet] : wallets) {
        file << id << "|" << wallet.getBalance() << "\n";
    }
    file.close();
}

std::string WalletManager::createWallet() {
    std::string id = "WALLET_" + std::to_string(wallets.size() + 1);
    wallets[id] = Wallet(id);
    saveWallets();
    return id;
}

bool WalletManager::walletExists(const std::string& id) {
    return wallets.find(id) != wallets.end();
}

Wallet& WalletManager::getWallet(const std::string& id) {
    return wallets.at(id);
}

bool WalletManager::transferPoints(const std::string& fromId, const std::string& toId, int points) {
    if (!walletExists(fromId) || !walletExists(toId)) return false;

    Wallet& fromWallet = wallets[fromId];
    Wallet& toWallet = wallets[toId];

    if (!fromWallet.deductPoints(points)) return false;

    toWallet.addPoints(points);
    std::string time = getCurrentTimestamp();

    Transaction tx{fromId, toId, points, time};
    fromWallet.addTransaction(tx);
    toWallet.addTransaction(tx);
    saveWallets();
    return true;
}

int WalletManager::getBalance(const std::string& walletId) {
    return walletExists(walletId) ? wallets[walletId].getBalance() : -1;
}

std::vector<Transaction> WalletManager::getTransactionHistory(const std::string& walletId) {
    if (!walletExists(walletId)) return {};
    return wallets[walletId].getHistory();
}
