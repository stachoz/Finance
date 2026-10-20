#include "RsiStrategy.h"

#include <list>
#include <cmath>

StrategyOutput RsiStrategy::proceed(const DOHLCV &data) {
    if (prices_history.empty()) {
        prices_history.emplace_front(data.close, 0);
        return StrategyOutput{Signal::NONE};
    }

    double diff = data.close - prices_history.front().price;
    if (diff > 0) {
        gains_sum += diff;
    }
    if (diff < 0) {
        losses_sum += std::abs(diff);
    }
    prices_history.emplace_front(data.close, diff);

    if (prices_history.size() > tracking_period) {
        const auto to_remove_price_diff = prices_history.back().price_change;
        if (to_remove_price_diff > 0) {
            gains_sum -= to_remove_price_diff;
        }
        else if (to_remove_price_diff < 0) {
            losses_sum -= std::abs(to_remove_price_diff);
        }
        prices_history.pop_back();
    }
    else {
        return StrategyOutput{Signal::NONE};
    }

    const double avg_gain = gains_sum / tracking_period;
    const double avg_loss = losses_sum / tracking_period;

    double rsi;
    if (avg_gain == 0.0 && avg_loss == 0.0) {
        rsi = 50.0;
    }
    else if (avg_loss == 0.0) {
        rsi = 100.0;
    }
    else if (avg_gain == 0.0) {
        rsi = 0.0;
    }
    else {
        const double rs = avg_gain / avg_loss;
        rsi = 100 - (100 / (1 + rs));
    }

    auto sig = Signal::NONE;

    if (rsi >= upper) {
        sig = Signal::SELL;
    }
    else if (rsi <= lower) {
        sig = Signal::BUY;
    }

    return StrategyOutput {sig};
}
