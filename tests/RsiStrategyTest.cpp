#include <gtest/gtest.h>

#include "TimeSeries.h"
#include "strategy/rsi/RsiStrategy.h"

namespace {
DOHLCV price(double close) {
    return {{}, 0, 0, 0, close, 0};
}
}

TEST(RsiStrategyTest, ReturnsNoneDuringWarmup) {
    RsiStrategy strategy{3};

    EXPECT_EQ(strategy.proceed(price(100.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(101.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(100.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(101.0)).signal, Signal::NONE);
}

TEST(RsiStrategyTest, SellsAfterSustainedPriceIncreases) {
    RsiStrategy strategy{3};

    EXPECT_EQ(strategy.proceed(price(100.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(102.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(101.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(104.0)).signal, Signal::SELL);
}

TEST(RsiStrategyTest, BuysAfterSustainedPriceDecreases) {
    RsiStrategy strategy{3};

    EXPECT_EQ(strategy.proceed(price(100.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(98.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(96.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(94.0)).signal, Signal::BUY);
}

TEST(RsiStrategyTest, ReturnsNoneWhenPricesAreFlat) {
    RsiStrategy strategy{3};

    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(strategy.proceed(price(100.0)).signal, Signal::NONE);
    }
}
