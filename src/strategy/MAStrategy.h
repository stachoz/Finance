#pragma once

#include <deque>

#include "IStrategy.h"
#include "TimeSeries.h"

class MAStrategy : public IStrategy{
public:

    MAStrategy(size_t fast_period, size_t slow_period);

    ~MAStrategy() override = default;

    StrategyOutput proceed(const DOHLCV& dohlcv) override;

private:

    /**
     * @brief Determines the trading signal based on the crossover of two moving averages.
     *
     * This function compares the previous and current values of the fast and slow
     * moving averages to detect a crossover event. A BUY signal is generated when the
     * fast moving average crosses above the slow one. A SELL signal is generated on
     * the reverse cross.
     *
     * @param prev_fast The previous value of the fast moving average.
     * @param curr_fast The current value of the fast moving average.
     * @param prev_slow The previous value of the slow moving average.
     * @param curr_slow The current value of the slow moving average.
     * @return Signal::BUY if the fast MA crosses above the slow MA.
     *         Signal::SELL if the fast MA crosses below the slow MA.
     *         Signal::NONE otherwise.
     */
    static Signal get_signal(double prev_fast, double curr_fast, double prev_slow, double curr_slow);

    static void update_window(std::deque<double> &window, double &sum, double new_value, size_t size);

    /**
     * @brief Calculates the precise price and time of a moving average crossover.
     *
     * @details This function treats the change in the fast and slow moving averages
     * over a single time step as two distinct line segments. It then solves the
     * system of linear equations for these two lines to find their intersection point.
     * This provides a more accurate price and time for the crossover event than
     * simply using the candle's closing price.
     *
     * @param prev_fast The value of the fast MA at the previous time step (t=0).
     * @param curr_fast The value of the fast MA at the current time step (t=1).
     * @param prev_slow The value of the slow MA at the previous time step (t=0).
     * @param curr_slow The value of the slow MA at the current time step (t=1).
     * @return A std::pair containing:
     *         - first: The price at which the two moving averages intersected.
     *         - second: The fractional time 't' of the intersection (between 0.0 and 1.0).
     */
    static std::pair<double, double> calculate_intersection_price(double prev_fast, double curr_fast, double prev_slow,
                                                                  double curr_slow);

    size_t fast_period;
    size_t slow_period;
    std::deque<double> fast_window {};
    std::deque<double> slow_window {};
    double fast_sum {0.0};
    double slow_sum {0.0};
};
