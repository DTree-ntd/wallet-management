#include "DataManager.h"
#include <filesystem>
#include <sstream>
#include <iostream>

void DataManager::ensureDataDirectory() {
    std::filesystem::create_directories("data");
}

bool DataManager::saveWallet(const Wallet& wallet) {
    ensureDataDirectory();
    std::ofstream file(WALLETS_FILE, std::ios::app);
    if (!file.is_open()) {
        return false;
    }

    file << wallet.getWalletId() << ","
         << wallet.getBalance() << ","
         << static_cast<int>(wallet.getType()) << "\n";
    
    return true;
}

bool DataManager::saveWallets(const std::vector<Wallet>& wallets) {
    ensureDataDirectory();
    std::ofstream file(WALLETS_FILE);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& wallet : wallets) {
        file << wallet.getWalletId() << ","
             << wallet.getBalance() << ","
             << static_cast<int>(wallet.getType()) << "\n";
    }
    
    return true;
}

bool DataManager::loadWallets(std::vector<Wallet>& wallets) {
    std::ifstream file(WALLETS_FILE);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string walletId;
        int balance;
        int type;
        char comma;

        ss >> walletId >> comma >> balance >> comma >> type;

        Wallet wallet(walletId);
        wallet.addPoints(balance);
        wallets.push_back(wallet);
    }

    return true;
}

Wallet* DataManager::findWalletById(const std::string& walletId) {
    std::vector<Wallet> wallets;
    if (!loadWallets(wallets)) {
        return nullptr;
    }

    for (auto& wallet : wallets) {
        if (wallet.getWalletId() == walletId) {
            return &wallet;
        }
    }

    return nullptr;
}

bool DataManager::saveTransaction(const Transaction& transaction) {
    ensureDataDirectory();
    std::ofstream file(TRANSACTIONS_FILE, std::ios::app);
    if (!file.is_open()) {
        return false;
    }

    file << transaction.transactionId << ","
         << transaction.fromWalletId << ","
         << transaction.toWalletId << ","
         << transaction.points << ","
         << transaction.timestamp << ","
         << transaction.status << "\n";
    
    return true;
}

bool DataManager::saveTransactions(const std::vector<Transaction>& transactions) {
    ensureDataDirectory();
    std::ofstream file(TRANSACTIONS_FILE);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& transaction : transactions) {
        file << transaction.transactionId << ","
             << transaction.fromWalletId << ","
             << transaction.toWalletId << ","
             << transaction.points << ","
             << transaction.timestamp << ","
             << transaction.status << "\n";
    }
    
    return true;
}

bool DataManager::loadTransactions(std::vector<Transaction>& transactions) {
    std::ifstream file(TRANSACTIONS_FILE);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string transactionId, fromWalletId, toWalletId, status;
        int points;
        std::time_t timestamp;
        char comma;

        ss >> transactionId >> comma
           >> fromWalletId >> comma
           >> toWalletId >> comma
           >> points >> comma
           >> timestamp >> comma
           >> status;

        transactions.emplace_back(transactionId, fromWalletId, toWalletId, 
                                points, timestamp, status);
    }

    return true;
}

std::vector<Transaction> DataManager::getTransactionsByWalletId(const std::string& walletId) {
    std::vector<Transaction> allTransactions;
    std::vector<Transaction> walletTransactions;
    
    if (!loadTransactions(allTransactions)) {
        return walletTransactions;
    }

    for (const auto& transaction : allTransactions) {
        if (transaction.fromWalletId == walletId || transaction.toWalletId == walletId) {
            walletTransactions.push_back(transaction);
        }
    }

    return walletTransactions;
}

bool DataManager::clearAllData() {
    try {
        std::filesystem::remove(WALLETS_FILE);
        std::filesystem::remove(TRANSACTIONS_FILE);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error clearing data: " << e.what() << std::endl;
        return false;
    }
} 