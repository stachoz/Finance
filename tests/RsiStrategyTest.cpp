#include <gtest/gtest.h>

#include "TimeSeries.h"
#include "strategy/rsi/RsiStrategy.h"

namespace {
DOHLCV price(double close) {
    return {{}, 0, 0, 0, close, 0};
}
}

TEST(RsiStrategyTest, ReturnsNoneDuringWarmup) {
    RsiStrategy strategy{3, RsiStrategy::StrategyVariant::BugEachTimeInSection};

    EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(101.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(101.0))->signal, Signal::NONE);
}

TEST(RsiStrategyTest, SellsAfterSustainedPriceIncreases) {
    RsiStrategy strategy{3, RsiStrategy::StrategyVariant::BugEachTimeInSection};

    EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(102.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(101.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(104.0))->signal, Signal::SELL);
}

TEST(RsiStrategyTest, BuysAfterSustainedPriceDecreases) {
    RsiStrategy strategy{3, RsiStrategy::StrategyVariant::BugEachTimeInSection};

    EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(98.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(96.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(94.0))->signal, Signal::BUY);
}

TEST(RsiStrategyTest, ReturnsNoneWhenPricesAreFlat) {
    RsiStrategy strategy{3, RsiStrategy::StrategyVariant::BugEachTimeInSection};

    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    }
}

TEST(RsiStrategyTest, SellsOnlyOnceUntilRsiReturnsToNeutral) {
    RsiStrategy strategy{3, RsiStrategy::StrategyVariant::BuyOnceInSection};

    EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(102.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(104.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(106.0))->signal, Signal::SELL);
    EXPECT_EQ(strategy.proceed(price(108.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(110.0))->signal, Signal::NONE);

    // A neutral RSI re-arms selling.
    EXPECT_EQ(strategy.proceed(price(108.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(110.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(112.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(114.0))->signal, Signal::SELL);
}

TEST(RsiStrategyTest, BuysOnlyOnceUntilRsiReturnsToNeutral) {
    RsiStrategy strategy{3, RsiStrategy::StrategyVariant::BuyOnceInSection};

    EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(98.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(96.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(94.0))->signal, Signal::BUY);
    EXPECT_EQ(strategy.proceed(price(92.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(90.0))->signal, Signal::NONE);

    // A neutral RSI re-arms buying.
    EXPECT_EQ(strategy.proceed(price(92.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(90.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(88.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(86.0))->signal, Signal::BUY);
}

TEST(RsiStrategyTest, SellsWhenPriceJumpsFromBuyRangeToSellRange) {
    RsiStrategy strategy{3, RsiStrategy::StrategyVariant::BuyOnceInSection};

    EXPECT_EQ(strategy.proceed(price(100.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(99.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(98.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(97.0))->signal, Signal::BUY);
    EXPECT_EQ(strategy.proceed(price(1000.0))->signal, Signal::SELL);
}
