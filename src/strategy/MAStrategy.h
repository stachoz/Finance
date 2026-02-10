#pragma once

#include <deque>

#include "../Statisitcs.h"
#include "../TimeSeries.h"

class MAStrategy {
public:
    MAStrategy(size_t fast_period, size_t slow_period);

    enum class Signal : uint8_t {
        BUY,
        SELL,
        NONE
    };

    Signal proceed(const DOHLCV& dohlcv);

private:
    static Signal get_signal(double prev_fast, double curr_fast, double prev_slow, double curr_slow);

    static void update_window(std::deque<double> &window, double &sum, double new_value, size_t size);

    size_t fast_period;
    size_t slow_period;
    std::deque<double> fast_window {};
    std::deque<double> slow_window {};
    double fast_sum {0.0};
    double slow_sum {0.0};
};
