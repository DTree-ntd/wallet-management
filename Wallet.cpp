#include "Wallet.h"

Wallet::Wallet() : walletId(""), balance(0) {}
Wallet::Wallet(const std::string& id, int initialBalance) 
    : walletId(id), balance(initialBalance) {}

std::string Wallet::getId() const { return walletId; }
int Wallet::getBalance() const { return balance; }
const std::vector<Transaction>& Wallet::getHistory() const { return history; }

void Wallet::addPoints(int amount) { balance += amount; }
bool Wallet::deductPoints(int amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}
void Wallet::addTransaction(const Transaction& tx) {
    history.push_back(tx);
}
