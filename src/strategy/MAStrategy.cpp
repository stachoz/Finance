#include "MAStrategy.h"

MAStrategy::MAStrategy(size_t fast_period, size_t slow_period):
    fast_period(fast_period),
    slow_period(slow_period) {
    if (fast_period > slow_period) {
        throw std::runtime_error("MAStrategy: first ma must by shorter then second");
    }
}

StrategyOutput MAStrategy::proceed(const DOHLCV& dohlcv) {
    const double closing_price = dohlcv.close;

    const double prev_fast = fast_window.empty() ? 0 : fast_sum / fast_window.size();
    const double prev_slow = slow_window.empty() ? 0 : slow_sum / slow_window.size();

    update_window(fast_window, fast_sum, closing_price, fast_period);
    update_window(slow_window, slow_sum, closing_price, slow_period);

    // loading period
    if (fast_window.size() < fast_period || slow_window.size() < slow_period) {
        return {Signal::NONE, 0, 0, 0};
    }

    const double curr_fast = fast_sum / fast_window.size();
    const double curr_slow = slow_sum / slow_window.size();

    const Signal signal = get_signal(prev_fast, curr_fast, prev_slow, curr_slow);
    std::pair<double, double> cross_price_t;

    if (signal != Signal::NONE) {
        cross_price_t = calculate_intersection_price(prev_fast, curr_fast, prev_slow, curr_slow);
    }

    return {
        signal,
        curr_fast,
        curr_slow,
        cross_price_t.first,
        cross_price_t.second,
    };
}

Signal MAStrategy::get_signal(double prev_fast, double curr_fast, double prev_slow, double curr_slow) {
    if (prev_fast <= prev_slow && curr_fast > curr_slow) {
        return Signal::BUY;
    }
    if (prev_fast >= prev_slow && curr_fast < curr_slow) {
        return Signal::SELL;
    }

    return Signal::NONE;
}

void MAStrategy::update_window(std::deque<double>& window, double& sum, double new_value, size_t size) {
    if (window.size() == size) {
        sum -= window.front();
        window.pop_front();
    }

    window.push_back(new_value);
    sum += new_value;
}

std::pair<double, double> MAStrategy::calculate_intersection_price(double prev_fast, double curr_fast, double prev_slow,
                                                                   double curr_slow) {
    const double t = (prev_slow - prev_fast) / ((curr_fast - prev_fast) - (curr_slow - prev_slow));

    return {prev_fast + t * (curr_fast - prev_fast), t};
}
