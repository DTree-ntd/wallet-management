#include "TransactionManager.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

bool TransactionManager::transferPoints(User& fromUser, User& toUser, int points) {
    // Lấy ví của người gửi và người nhận
    Wallet& fromWallet = fromUser.getWallet();
    Wallet& toWallet = toUser.getWallet();

    // Kiểm tra số dư
    if (!fromWallet.deductPoints(points)) {
        return false;
    }

    // Thêm điểm vào ví đích
    toWallet.addPoints(points);

    // Tạo giao dịch mới
    std::string transactionId = generateTransactionId();
    Transaction transaction(
        transactionId,
        fromWallet.getWalletId(),
        toWallet.getWalletId(),
        points,
        std::time(nullptr),
        "COMPLETED"
    );

    // Thêm vào lịch sử giao dịch của cả hai ví
    fromWallet.addTransaction(transaction);
    toWallet.addTransaction(transaction);

    return true;
}

std::string TransactionManager::generateTransactionId() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    );
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);
    
    std::stringstream ss;
    ss << "TXN" << now_ms.count() << std::setw(4) << std::setfill('0') << dis(gen);
    return ss.str();
}
