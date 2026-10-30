#pragma once


#include <list>
#include <stdexcept>

#include "strategy/IStrategy.h"

class RsiStrategy : public IStrategy{
public:
    enum class StrategyVariant : uint8_t {
        BuyOnceInSection,
        BugEachTimeInSection,
        ScaledPosition
    };

    ~RsiStrategy() override = default;
    explicit RsiStrategy(uint8_t tracking_period_value, StrategyVariant variant = StrategyVariant::BuyOnceInSection) : tracking_period(tracking_period_value),
    strategy_variant(variant){
        if (tracking_period == 0) {
            throw std::invalid_argument("expected value > 0");
        }
    };

    std::unique_ptr<StrategyOutput> proceed(const DOHLCV &data) override;

    std::string_view name() const override;

private:
    Signal getSignal(double rsi);

    struct PriceEntry{
        double price;
        double price_change; // value > 0 is gain, value < 0 loss
    };
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
