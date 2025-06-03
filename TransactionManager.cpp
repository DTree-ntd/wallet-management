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
