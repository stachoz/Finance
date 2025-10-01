#include <gtest/gtest.h>
#include "Statisitcs.h"
#include <limits>


TEST(StatisticsTests, Returns_Basic) {
    std::vector<double> values {100, 110, 121};
    auto result = stats::returns(values);
    ASSERT_EQ(result.size(), 2);
    EXPECT_NEAR(result[0], 0.10, 1e-9);
    EXPECT_NEAR(result[1], 0.10, 1e-9);
}

TEST(StatisticsTests, Returns_NotEnoughData) {
    std::vector<double> values {100};
    auto result = stats::returns(values);
    EXPECT_TRUE(result.empty());
}

TEST(StatisticsTests, LogReturns_Basic) {
    std::vector<double> values {100, 110};
    auto result = stats::log_returns(values);
    ASSERT_EQ(result.size(), 1);
    EXPECT_NEAR(result[0], std::log(110.0 / 100.0), 1e-9);
}

TEST(StatisticsTests, meanSimpleTest) {
    const std::vector values {2, 2};
    const auto result = stats::mean(values);
    ASSERT_EQ(result, 2);
}

TEST(StatisticsTests, emptyValuesTest) {
    std::vector<int> values {};
    const auto result = stats::mean(values);
    ASSERT_EQ(result, 0);
}

TEST(StatisticsTests, Mean_Basic) {
    std::vector<int> values{1, 2, 3, 4};
    auto result = stats::mean(values);
    EXPECT_DOUBLE_EQ(result, 2.5);
}

TEST(StatisticsTests, Mean_Empty) {
    std::vector<int> values{};
    auto result = stats::mean(values);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(StatisticsTests, MovingAverage_Basic) {
    std::vector<int> values{1, 2, 3, 4, 5};
    auto result = stats::moving_average(values, 3);
    ASSERT_EQ(result.size(), 3);
    EXPECT_DOUBLE_EQ(result[0], 2.0);
    EXPECT_DOUBLE_EQ(result[1], 3.0);
    EXPECT_DOUBLE_EQ(result[2], 4.0);
}

TEST(StatisticsTests, MovingAverage_WindowTooLarge) {
    std::vector<int> values {1, 2};
    EXPECT_THROW(stats::moving_average(values, 3), std::invalid_argument);
}

TEST(StatisticsTests, Variance_Basic) {
    std::vector<double> values{1, 2, 3};
    auto result = stats::variance(values);
    EXPECT_NEAR(result, 2.0/3.0, 1e-9);
}

TEST(StatisticsTests, StdDeviation_Basic) {
    std::vector<double> values{1, 2, 3};
    auto result = stats::std_deviation(values);
    EXPECT_NEAR(result, std::sqrt(2.0/3.0), 1e-9);
}

TEST(StatisticsTests, Covariance_Basic) {
    std::vector<double> x {1, 2, 3};
    std::vector<double> y {2, 4, 6};
    auto result = stats::covariance(x, y);
    EXPECT_NEAR(result, 2.0, 1e-9);
}

TEST(StatisticsTests, Covariance_SizeMismatch) {
    std::vector<double> x{1, 2};
    std::vector<double> y{1};
    EXPECT_THROW(stats::covariance(x, y), std::invalid_argument);
}

