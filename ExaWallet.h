#pragma once

#include <memory>
#include <unordered_set>

#include <wallet2_api.h>

class ExaChain;

class ExaWallet : public Monero::WalletListener {
public:
    ExaWallet(ExaChain *api, Monero::Wallet *wallet);

    ExaWallet(const ExaWallet&) = delete;

    ~ExaWallet();

    uint64_t Balance();
    std::string DisplayBalance();

    void ImportMultisigInfo();
    void WaitSync();
public:
    void moneySpent(const std::string &txId, uint64_t amount) override;
    void moneyReceived(const std::string &txId, uint64_t amount) override;
    
    void unconfirmedMoneyReceived(const std::string &txId, uint64_t amount) override;
    void newBlock(uint64_t height) override;

    void updated() override;

    void refreshed() override;
private:
    Monero::Wallet *wallet_;
    ExaChain *api_;
    std::time_t last_refreshed_;
    std::condition_variable refreshed_;

    friend class ExaChain;
};

class ExaChain {
public:
    ExaChain(const char* daemon_address_ = nullptr);
    ExaChain &SetLogLevel();
    ExaChain &SetTestNet(bool testnet);
    ExaWallet* OpenWallet(const char *path, const char *password);
    ExaWallet* CreateWallet(const char *path, const char *password);

protected:
    void InitWallet(ExaWallet *wallet);
private:
    Monero::NetworkType networkType_;
    Monero::WalletManager *manager_;

    std::mutex gate_;

    std::string daemon_address_;
    std::list<std::unique_ptr<ExaWallet>> wallets_;
    friend class ExaWallet;
};

