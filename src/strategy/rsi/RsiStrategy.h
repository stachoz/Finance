#pragma once

#include <list>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "strategy/IStrategy.h"

class RsiStrategy : public IStrategy{
public:
    enum class StrategyVariant : uint8_t {
        BuyOnceInSection,
        BuyEachTimeInSection,
        ScaledPosition
    };

    ~RsiStrategy() override = default;
    RsiStrategy(double transaction_size, uint8_t tracking_period_value, StrategyVariant variant) : transaction_size(transaction_size) ,tracking_period(tracking_period_value),
    strategy_variant(variant){
        if (tracking_period == 0) {
            throw std::invalid_argument("expected value > 0");
        }
    };

    std::unique_ptr<StrategyOutput> proceed(const DOHLCV &data) override;

    std::string_view name() const override;

private:
    std::pair<Signal, double> getSignalAndTransactionSize(double rsi);

    struct PriceEntry{
        double price;
        double price_change; // value > 0 is gain, value < 0 loss
    };

    std::vector<std::pair<double, double>> sell_rsi_threshold_scales= {
        {95, 0.50},
        {90, 0.25},
        {80, 0.25},
    };
    std::vector<std::pair<double, double>> buy_rsi_threshold_scales = {
        {20, 0.50},
        {25, 0.25},
        {30, 0.25}
    };
    double transaction_size;
    uint8_t tracking_period;
    StrategyVariant strategy_variant;
    const double lower = 30;
    const double upper = 80;
    std::list<PriceEntry> prices_history;
    double gains_sum = 0.0;
    double losses_sum = 0.0;
    bool has_sold_already = false;
    bool has_bought_already = false;
};
