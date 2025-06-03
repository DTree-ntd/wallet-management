#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <string>
#include "User.h"
#include <ctime>

class TransactionManager {
public:
    // Thực hiện chuyển điểm giữa 2 ví thuộc 2 user khác nhau hoặc cùng user
    // Trả về true nếu thành công, false nếu lỗi (ví dụ ví không đủ điểm)
    static bool transferPoints(User& fromUser, const std::string& fromWalletId,
                               User& toUser, const std::string& toWalletId,
                               int points);

    // Tạo mã giao dịch duy nhất (có thể đơn giản là timestamp + ngẫu nhiên)
    static std::string generateTransactionId();
};

#endif // TRANSACTIONMANAGER_H
