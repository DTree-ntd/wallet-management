#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include "Wallet.h"
#include "User.h"

class DataManager {
private:
    static const std::string WALLETS_FILE;
    static const std::string TRANSACTIONS_FILE;
    
    // Đảm bảo thư mục data tồn tại
    static void ensureDataDirectory();

public:
    // Lưu thông tin ví
    static bool saveWallet(const Wallet& wallet);
    static bool saveWallets(const std::vector<Wallet>& wallets);
    
    // Đọc thông tin ví
    static bool loadWallets(std::vector<Wallet>& wallets);
    static Wallet* findWalletById(const std::string& walletId);
    
    // Lưu giao dịch
    static bool saveTransaction(const Transaction& transaction);
    static bool saveTransactions(const std::vector<Transaction>& transactions);
    
    // Đọc giao dịch
    static bool loadTransactions(std::vector<Transaction>& transactions);
    static std::vector<Transaction> getTransactionsByWalletId(const std::string& walletId);
    
    // Xóa dữ liệu (chỉ dùng cho testing)
    static bool clearAllData();
};

#endif // DATA_MANAGER_H 