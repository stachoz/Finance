#include <gtest/gtest.h>
#include "Wallet.h"

class WalletTest : public ::testing::Test {
protected:
    const double SPREAD = 0.02;
    const double COMMISSION_PER_SHARE = 0.01;
    const double MINIMUM_COMMISSION = 1.0;

    const double INITIAL_SALDO = 1000.0;
    const double SHARES_PER_TRANSACTION = 10.0;

    Wallet wallet{INITIAL_SALDO, SHARES_PER_TRANSACTION};
    void SetUp() override {
        wallet.set_spread(SPREAD);
        wallet.set_commission_per_share(COMMISSION_PER_SHARE);
    }
};

TEST_F(WalletTest, BuyTransactionAppliesSpreadAndCommission) {
    constexpr double price = 50.0;
    const double execution_price = price + (SPREAD / 2.0); // 50.01
    const double commission = std::max(SHARES_PER_TRANSACTION * COMMISSION_PER_SHARE, MINIMUM_COMMISSION); // max(0.1, 1.0) = 1.0
    const double expected_cost = SHARES_PER_TRANSACTION * execution_price + commission; // 10 * 50.01 + 1.0 = 500.1 + 1.0 = 501.1

    const bool transaction_completed = wallet.update(Signal::BUY, price);

    EXPECT_TRUE(transaction_completed);
    EXPECT_DOUBLE_EQ(wallet.get_saldo(), INITIAL_SALDO - expected_cost);
    EXPECT_DOUBLE_EQ(wallet.get_owned_shares(), SHARES_PER_TRANSACTION);
}

TEST_F(WalletTest, SellTransactionAppliesSpreadAndCommission) {
    wallet.update(Signal::BUY, 50.0);
    const double saldo_after_buy = wallet.get_saldo();

    constexpr double sell_price = 60.0;
    const double execution_price = sell_price - (SPREAD / 2.0); // 59.99
    const double commission = std::max(SHARES_PER_TRANSACTION * COMMISSION_PER_SHARE, MINIMUM_COMMISSION); // 1.0
    const double expected_revenue = SHARES_PER_TRANSACTION * execution_price - commission; // 10 * 59.99 - 1.0 = 599.9 - 1.0 = 598.9

    const bool transaction_completed = wallet.update(Signal::SELL, sell_price);

    EXPECT_TRUE(transaction_completed);
    EXPECT_DOUBLE_EQ(wallet.get_saldo(), saldo_after_buy + expected_revenue);
    EXPECT_DOUBLE_EQ(wallet.get_owned_shares(), 0.0);
}

TEST_F(WalletTest, BuyFailsIfSaldoCannotCoverCostWithCommission) {
    constexpr double price = 99.9; // Base cost is 999.0
    // Execution price = 99.9 + 0.01 = 99.91
    // Total cost = 10 * 99.91 + 1.0 = 999.1 + 1.0 = 1000.1, which is > 1000.0

    const bool transaction_completed = wallet.update(Signal::BUY, price);

    EXPECT_FALSE(transaction_completed);
    EXPECT_DOUBLE_EQ(wallet.get_saldo(), INITIAL_SALDO);
    EXPECT_DOUBLE_EQ(wallet.get_owned_shares(), 0.0);
}

TEST_F(WalletTest, SellFailsWhenNoSharesAreOwned) {
    const bool transaction_completed = wallet.update(Signal::SELL, 50.0);
    EXPECT_FALSE(transaction_completed);
    EXPECT_DOUBLE_EQ(wallet.get_saldo(), INITIAL_SALDO);
}

TEST_F(WalletTest, CommissionIsCalculatedPerShareWhenGreaterThanMinimum) {
    Wallet large_wallet(10000.0, 200.0); // 200 shares per transaction
    large_wallet.set_spread(SPREAD);
    large_wallet.set_commission_per_share(COMMISSION_PER_SHARE);

    constexpr double price = 20.0;

    const double execution_price = price + (SPREAD / 2.0); // 20.01
    const double expected_commission = 200.0 * COMMISSION_PER_SHARE; // 2.0, which is > 1.0
    const double expected_cost = 200.0 * execution_price + expected_commission; // 4002.0 + 2.0 = 4004.0

    const bool transaction_completed = large_wallet.update(Signal::BUY, price);

    EXPECT_TRUE(transaction_completed);
    EXPECT_DOUBLE_EQ(large_wallet.get_saldo(), 10000.0 - expected_cost);
}

TEST_F(WalletTest, NetWorthUpdatesWithPriceEvenOnSignalNone) {
    wallet.update(Signal::BUY, 50.0);
    const double saldo_after_buy = wallet.get_saldo();

    constexpr double new_price = 100.0;
    wallet.update(Signal::NONE, new_price);

    const double expected_net_worth = saldo_after_buy + (SHARES_PER_TRANSACTION * new_price);
    EXPECT_DOUBLE_EQ(wallet.get_total_net_worth(), expected_net_worth);
}
