#include <gtest/gtest.h>

#include "TimeSeries.h"
#include "strategy/rsi/RsiStrategy.h"

class RsiStrategyTest : public ::testing::Test {
protected:
    static DOHLCV price(double p) {
        return {{}, 0, 0, 0, p, 0};
    }

    RsiStrategy eachTimeStrategy {30, 3,RsiStrategy::StrategyVariant::BuyEachTimeInSection};
    RsiStrategy onceStrategy{30, 3, RsiStrategy::StrategyVariant::BuyOnceInSection};
    RsiStrategy scaledPositionStrategy{30, 3, RsiStrategy::StrategyVariant::ScaledPosition};
};

TEST_F(RsiStrategyTest, ReturnsNoneDuringWarmup) {
    EXPECT_EQ(eachTimeStrategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(101.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(101.0))->signal, Signal::NONE);
}

TEST_F(RsiStrategyTest, SellsAfterSustainedPriceIncreases) {
    EXPECT_EQ(eachTimeStrategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(102.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(101.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(104.0))->signal, Signal::SELL);
}

TEST_F(RsiStrategyTest, BuysAfterSustainedPriceDecreases) {
    EXPECT_EQ(eachTimeStrategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(98.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(96.0))->signal, Signal::NONE);
    EXPECT_EQ(eachTimeStrategy.proceed(price(94.0))->signal, Signal::BUY);
}

TEST_F(RsiStrategyTest, ReturnsNoneWhenPricesAreFlat) {
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(eachTimeStrategy.proceed(price(100.0))->signal, Signal::NONE);
    }
}

TEST_F(RsiStrategyTest, SellsOnlyOnceUntilRsiReturnsToNeutral) {
    EXPECT_EQ(onceStrategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(102.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(104.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(106.0))->signal, Signal::SELL);
    EXPECT_EQ(onceStrategy.proceed(price(108.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(110.0))->signal, Signal::NONE);

    // A neutral RSI re-arms selling.
    EXPECT_EQ(onceStrategy.proceed(price(108.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(110.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(112.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(114.0))->signal, Signal::SELL);
}

TEST_F(RsiStrategyTest, BuysOnlyOnceUntilRsiReturnsToNeutral) {
    EXPECT_EQ(onceStrategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(98.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(96.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(94.0))->signal, Signal::BUY);
    EXPECT_EQ(onceStrategy.proceed(price(92.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(90.0))->signal, Signal::NONE);

    // A neutral RSI re-arms buying.
    EXPECT_EQ(onceStrategy.proceed(price(92.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(90.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(88.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(86.0))->signal, Signal::BUY);
}

TEST_F(RsiStrategyTest, SellsWhenPriceJumpsFromBuyRangeToSellRange) {
    EXPECT_EQ(onceStrategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(99.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(98.0))->signal, Signal::NONE);
    EXPECT_EQ(onceStrategy.proceed(price(97.0))->signal, Signal::BUY);
    EXPECT_EQ(onceStrategy.proceed(price(1000.0))->signal, Signal::SELL);
}

TEST_F(RsiStrategyTest, UsesMostOversoldThresholdAndScalesTransactionSize) {
    scaledPositionStrategy.proceed(price(100.0));
    scaledPositionStrategy.proceed(price(101.0));
    scaledPositionStrategy.proceed(price(100.0));
    const auto output = scaledPositionStrategy.proceed(price(98.0));

    EXPECT_EQ(output->signal, Signal::BUY);
    EXPECT_DOUBLE_EQ(output->transaction_size, 7.5); // 25% of 30 shares
}

TEST_F(RsiStrategyTest, UsesLargestScaleAtMostOversoldThreshold) {
    scaledPositionStrategy.proceed(price(100.0));
    scaledPositionStrategy.proceed(price(99.0));
    scaledPositionStrategy.proceed(price(98.0));
    const auto output = scaledPositionStrategy.proceed(price(97.0));

    EXPECT_EQ(output->signal, Signal::BUY);
    EXPECT_DOUBLE_EQ(output->transaction_size, 15.0); // 50% of 30 shares
}

TEST_F(RsiStrategyTest, ScalesOverboughtTransactionsAndEmitsOnlyOncePerSection) {
    scaledPositionStrategy.proceed(price(100.0));
    scaledPositionStrategy.proceed(price(101.0));
    scaledPositionStrategy.proceed(price(102.0));
    const auto sell = scaledPositionStrategy.proceed(price(103.0));

    ASSERT_EQ(sell->signal, Signal::SELL);
    EXPECT_DOUBLE_EQ(sell->transaction_size, 15.0);

    const auto repeated_sell = scaledPositionStrategy.proceed(price(104.0));
    EXPECT_EQ(repeated_sell->signal, Signal::NONE);
    EXPECT_DOUBLE_EQ(repeated_sell->transaction_size, 0.0);
}

TEST_F(RsiStrategyTest, ScaledPositionSellsAtTheBuyOnceOverboughtThreshold) {
    scaledPositionStrategy.proceed(price(100.0));
    scaledPositionStrategy.proceed(price(117.0));
    scaledPositionStrategy.proceed(price(116.0));
    const auto sell = scaledPositionStrategy.proceed(price(114.0));

    EXPECT_EQ(sell->signal, Signal::SELL);
    EXPECT_DOUBLE_EQ(sell->transaction_size, 7.5);
}