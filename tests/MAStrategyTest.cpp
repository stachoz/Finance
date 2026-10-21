#include <gtest/gtest.h>

#include "TimeSeries.h"
#include "strategy/ma/MAStrategy.h"


class MAStrategyTest : public ::testing::Test {
protected:
    static DOHLCV price(double p) {
        return {{}, 0, 0, 0, p, 0};
    }

    MAStrategy strategy {2, 4};
};

TEST_F(MAStrategyTest, ReturnsNoneDuringWarmup) {
    EXPECT_EQ(strategy.proceed(price(10.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(11.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(12.0))->signal, Signal::NONE);
    EXPECT_EQ(strategy.proceed(price(13.0))->signal, Signal::NONE);
}

TEST_F(MAStrategyTest, DetectsGoldenCross) {
    strategy.proceed(price(100));
    strategy.proceed(price(90));
    strategy.proceed(price(80));
    strategy.proceed(price(70));

    // Fast (2-period) will react much quicker than Slow (4-period)
    auto strategy_output = strategy.proceed(price(150));

    EXPECT_EQ(strategy_output->signal, Signal::BUY);

    const auto* ma_output = dynamic_cast<const MAStrategyOutput*>(strategy_output.get());
    ASSERT_NE(ma_output, nullptr);
    EXPECT_GE(ma_output->t, 0.0);
    EXPECT_LE(ma_output->t, 1.0);
}

TEST_F(MAStrategyTest, DetectsDeathCross) {
    strategy.proceed(price(10));
    strategy.proceed(price(20));
    strategy.proceed(price(30));
    strategy.proceed(price(40));

    // Price crashes
    auto strategy_output = strategy.proceed(price(5));

    ASSERT_NE(strategy_output, nullptr);
    EXPECT_EQ(strategy_output->signal, Signal::SELL);

    const auto* ma_output = dynamic_cast<const MAStrategyOutput*>(strategy_output.get());
    ASSERT_NE(ma_output, nullptr);
    EXPECT_GE(ma_output->t, 0.0);
    EXPECT_LE(ma_output->t, 1.0);
}

TEST(MAStrategyConstructor, ThrowsOnInvalidIntervals) {
    EXPECT_THROW(MAStrategy(50, 20), std::runtime_error);
}
