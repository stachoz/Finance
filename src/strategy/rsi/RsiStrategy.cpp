#include "RsiStrategy.h"

#include <list>
#include <cmath>

std::unique_ptr<StrategyOutput> RsiStrategy::proceed(const DOHLCV &data) {
    if (prices_history.empty()) {
        prices_history.emplace_front(data.close, 0);
        return std::make_unique<RsiStrategyOutput>(StrategyOutput{Signal::NONE, data.close, 0}, 0.0);
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
        return std::make_unique<RsiStrategyOutput>(StrategyOutput{Signal::NONE, data.close, 0}, 0.0);
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

    auto [signal, size] = getSignalAndTransactionSize(rsi);
    return std::make_unique<RsiStrategyOutput>(StrategyOutput{signal, data.close, size}, rsi);
}

std::string_view RsiStrategy::name() const {
    return "rsi";
}

std::pair<Signal, double> RsiStrategy::getSignalAndTransactionSize(double rsi) {
    const bool overbought = rsi >= upper;
    const bool oversold = rsi <= lower;

    switch (strategy_variant) {
        case StrategyVariant::BuyOnceInSection:
            if (overbought) {
                if (has_sold_already) {
                    return {Signal::NONE, 0};
                }
                has_sold_already = true;
                return {Signal::SELL, transaction_size};
            }
            if (oversold) {
                if (has_bought_already) {
                    return {Signal::NONE, 0};
                }
                has_bought_already = true;
                return {Signal::BUY, transaction_size};
            }

            has_bought_already = false;
            has_sold_already = false;
            return {Signal::NONE, 0};
        case StrategyVariant::BuyEachTimeInSection:
            if (overbought) {
                return {Signal::SELL, transaction_size};
            }
            if (oversold) {
                return {Signal::BUY, transaction_size};
            }
            return {Signal::NONE, 0};
        case StrategyVariant::ScaledPosition:
            const auto overbought_pos_scale =  std::ranges::find_if(sell_rsi_threshold_scales,
                [rsi](const std::pair<double, double>& threshold_scale) {
                return rsi >= threshold_scale.first;
            });

            const auto oversold_pos_scale =  std::ranges::find_if(buy_rsi_threshold_scales,
                [rsi](const std::pair<double, double>& threshold_scale) {
                return rsi <= threshold_scale.first;
            });

            if (overbought_pos_scale != std::end(sell_rsi_threshold_scales)) {
                if (has_sold_already) {
                    return {Signal::NONE, 0};
                }

                has_sold_already = true;
                return {Signal::SELL, transaction_size * overbought_pos_scale->second};
            }

            if (oversold_pos_scale != std::end(buy_rsi_threshold_scales)) {
                if (has_bought_already) {
                    return {Signal::NONE, 0};
                }

                has_bought_already = true;
                return {Signal::BUY, transaction_size * oversold_pos_scale->second};
            }

            has_bought_already = false;
            has_sold_already = false;
            return {Signal::NONE, 0};
    }

    return {Signal::NONE, 0};
}
