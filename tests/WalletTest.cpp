#include <gtest/gtest.h>

#include "Wallet.h"

class WalletTest : public ::testing::Test {
protected:
    const double INITIAL_SALDO = 1000.0;
    const double SHARES_PER_TRANSACTION = 10.0;

    Wallet wallet {INITIAL_SALDO, SHARES_PER_TRANSACTION};
};

TEST_F(WalletTest, SuccessfulBuy) {
    constexpr double price = 50.0;
    const double expected_cost = SHARES_PER_TRANSACTION * price;

    wallet.update(Signal::BUY, price);

    EXPECT_DOUBLE_EQ(wallet.get_saldo(), INITIAL_SALDO - expected_cost);
    EXPECT_DOUBLE_EQ(wallet.get_owned_shares(), SHARES_PER_TRANSACTION);
}

TEST_F(WalletTest, BuyFailsWhenNotEnoughSaldo) {
    constexpr double expensive_price = 200.0;

    wallet.update(Signal::BUY, expensive_price);

    EXPECT_DOUBLE_EQ(wallet.get_saldo(), INITIAL_SALDO);
    EXPECT_DOUBLE_EQ(wallet.get_owned_shares(), 0.0);
}

TEST_F(WalletTest, SuccessfulSellIncreasesSaldoAndDecreasesShares) {
    wallet.update(Signal::BUY, 50.0);
    wallet.update(Signal::SELL, 60.0);

    EXPECT_DOUBLE_EQ(wallet.get_saldo(), 500.0 + 600.0);
    EXPECT_DOUBLE_EQ(wallet.get_owned_shares(), 0.0);
}

TEST_F(WalletTest, NetWorthUpdatesWithPriceEvenOnSignalNone) {
    wallet.update(Signal::BUY, 50.0);

    wallet.update(Signal::NONE, 100.0);

    constexpr double expected_net_worth = 500.0 + (10.0 * 100.0);
    EXPECT_DOUBLE_EQ(wallet.get_total_net_worth(), expected_net_worth);
}