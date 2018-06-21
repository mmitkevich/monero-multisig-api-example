#include <memory>

#include "ExaWallet.h"


ExaChain::ExaChain(const char* daemon_address) :
    networkType_ (Monero::NetworkType::MAINNET),
    manager_(Monero::WalletManagerFactory::getWalletManager()),
    daemon_address_(daemon_address ? daemon_address : "monero1.exan.tech:18081")
{
}

ExaWallet* ExaChain::OpenWallet(const char *path, const char *password) {
    ExaWallet *result = new ExaWallet(this, manager_->openWallet(path, password, networkType_));
    InitWallet(result);
    result->wallet_->startRefresh();
    wallets_.push_back(std::unique_ptr<ExaWallet>(result));
    return result;
}


ExaWallet* ExaChain::CreateWallet(const char *path, const char *password) {
    ExaWallet *result =  new ExaWallet(this, manager_->createWallet(path, password, "en", networkType_));
    InitWallet(result);
    wallets_.push_back(std::unique_ptr<ExaWallet>(result));
    return result;
}

void ExaChain::InitWallet(ExaWallet *wallet) {
    wallet->wallet_->init(daemon_address_);
    wallet->wallet_->setTrustedDaemon(true);
}
ExaChain & ExaChain::SetLogLevel() {
    Monero::WalletManagerFactory::setLogLevel(Monero::WalletManagerFactory::LogLevel_Min);
    return *this;
}

ExaChain & ExaChain::SetTestNet(bool testnet) {
    networkType_ = testnet ? Monero::NetworkType::TESTNET : Monero::NetworkType::MAINNET;
}

/////////////////

ExaWallet::ExaWallet(ExaChain *api, Monero::Wallet *wallet) :
    api_(api),
    wallet_(wallet),
    last_refreshed_(0)
{
    wallet_->setListener(this);
}
ExaWallet::~ExaWallet() {
    if(wallet_) {
        refreshed_.notify_all();
        api_->manager_->closeWallet(wallet_);
        wallet_ = nullptr;
    }
}

uint64_t ExaWallet::Balance() {
    return wallet_->balance();
}

std::string ExaWallet::DisplayBalance() {
    return Monero::Wallet::displayAmount(wallet_->balance());
}

void ExaWallet::WaitSync() {
    std::unique_lock<std::mutex> l(api_->gate_);
    refreshed_.wait(l);
    l.unlock();
}

void ExaWallet::moneySpent(const std::string &txId, uint64_t amount) 
{

}

void ExaWallet::moneyReceived(const std::string &txId, uint64_t amount) 
{

}

void ExaWallet::unconfirmedMoneyReceived(const std::string &txId, uint64_t amount) 
{

}
void ExaWallet::newBlock(uint64_t height) 
{

}

void ExaWallet::updated() 
{
}

void ExaWallet::refreshed() 
{
    last_refreshed_ = std::time(nullptr);
    refreshed_.notify_all();
}


