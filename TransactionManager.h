#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <string>
#include "User.h"
#include <ctime>

class TransactionManager {
public:
    static const std::string TOTAL_WALLET_ID;

    // Thực hiện chuyển điểm giữa 2 user
    // Trả về true nếu thành công, false nếu lỗi (ví dụ ví không đủ điểm)
    static bool transferPoints(User& fromUser, User& toUser, int points);

    // Tạo mã giao dịch duy nhất
    static std::string generateTransactionId();

    // Kiểm tra xem có phải là giao dịch với ví tổng không
    static bool isTotalWalletTransaction(const std::string& walletId) {
        return walletId == TOTAL_WALLET_ID;
    }

    // Kiểm tra quyền thực hiện giao dịch với ví tổng
    static bool canInteractWithTotalWallet(const User& user) {
        return user.getIsAdmin();
    }
};

#endif // TRANSACTIONMANAGER_H
