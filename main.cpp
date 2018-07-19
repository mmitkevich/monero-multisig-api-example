#include <memory>
#include <iostream>
#include <string>
#include "src/wallet/api/wallet2_api.h"
#include "contrib/epee/include/misc_log_ex.h"

int main(int argc, char ** argv) {
    Monero::WalletManagerFactory::setLogLevel(Monero::WalletManagerFactory::LogLevel_Max);
    
    auto walletManager = std::unique_ptr<Monero::WalletManager>(Monero::WalletManagerFactory::getWalletManager());

    auto wallet = std::unique_ptr<Monero::Wallet>(walletManager->createWallet("test-wallet", "123", "en", Monero::NetworkType::MAINNET));
}