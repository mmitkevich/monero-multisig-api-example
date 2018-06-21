#include <iostream>

#include "ExaWallet.h"

int main(int argc, char* argv[]) {
    ExaChain chain;
    auto wallet = chain.OpenWallet("/Users/mike/Downloads/wallet/mainnet-1","123");
    wallet->WaitSync();

    std::cout << "\n***************\n\n\n\nBALANCE = " <<  wallet->DisplayBalance() << "\n\n\n***********\n";

    return 0;
}