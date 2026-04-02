#include <gtest/gtest.h>

#include "TimeSeries.h"
#include "strategy/MAStrategy.h"


class MAStrategyTest : public ::testing::Test {
protected:
    static DOHLCV price(double p) {
        return {{}, 0, 0, 0, p, 0};
    }

    MAStrategy strategy {2, 4};
};

TEST_F(MAStrategyTest, ReturnsNoneDuringWarmup) {
    EXPECT_EQ(strategy.proceed(price(10.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(11.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(12.0)).signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(13.0)).signal, Signal::NONE);
}

TEST_F(MAStrategyTest, DetectsGoldenCross) {
    strategy.proceed(price(100));
    strategy.proceed(price(90));
    strategy.proceed(price(80));
    strategy.proceed(price(70));

    // Fast (2-period) will react much quicker than Slow (4-period)
    const auto strategy_output = strategy.proceed(price(150));

    EXPECT_EQ(strategy_output.signal, Signal::BUY);

    // t should be in range <0, 1>
    EXPECT_GE(strategy_output.t, 0.0);
    EXPECT_LE(strategy_output.t, 1.0);
}

TEST_F(MAStrategyTest, DetectsDeathCross) {
    strategy.proceed(price(10));
    strategy.proceed(price(20));
    strategy.proceed(price(30));
    strategy.proceed(price(40));

    // Price crashes
    const auto strategy_output = strategy.proceed(price(5));

    EXPECT_EQ(strategy_output.signal, Signal::SELL);
    EXPECT_NEAR(strategy_output.price, 5.0, 0.001);

    // T must be valid
    EXPECT_GE(strategy_output.t, 0.0);
    EXPECT_LE(strategy_output.t, 1.0);
}

TEST(MAStrategyConstructor, ThrowsOnInvalidIntervals) {
    EXPECT_THROW(MAStrategy(50, 20), std::runtime_error);
}