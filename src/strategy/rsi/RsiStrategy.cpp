#include "RsiStrategy.h"

#include <list>
#include <cmath>
#include <regex.h>

std::unique_ptr<StrategyOutput> RsiStrategy::proceed(const DOHLCV &data) {
    if (prices_history.empty()) {
        prices_history.emplace_front(data.close, 0);
        return std::make_unique<RsiStrategyOutput>(Signal::NONE, data.close, 0.0);
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
        return std::make_unique<RsiStrategyOutput>(Signal::NONE, data.close, 0.0);
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

    return std::make_unique<RsiStrategyOutput>(getSignal(rsi), data.close, rsi);
}

std::string_view RsiStrategy::name() const {
    return "rsi";
}

Signal RsiStrategy::getSignal(double rsi) {
    switch (strategy_variant) {
        case StrategyVariant::BuyOnceInSection:
            if (rsi >= upper && !has_sold_already) {
                has_sold_already = true;
                return Signal::SELL;
            }
            if (rsi <= lower && !has_bought_already) {
                has_bought_already = true;
                return Signal::BUY;
            }
            if (rsi > lower && rsi < upper) { // neutral range resets transaction flags
                has_bought_already = false;
                has_sold_already = false;
                return Signal::NONE;
            }
            break;
        case StrategyVariant::BugEachTimeInSection:
            if (rsi >= upper) {
                return Signal::SELL;
            }
            if (rsi <= lower) {
                return Signal::BUY;
            }
            return Signal::NONE;
        case StrategyVariant::ScaledPosition: // TODO
            return Signal::NONE;
    }

    return Signal::NONE;
}
