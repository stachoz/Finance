#pragma once


#include <list>
#include <stdexcept>

#include "strategy/IStrategy.h"

class RsiStrategy : public IStrategy{
public:
    ~RsiStrategy() override = default;
    explicit  RsiStrategy(uint8_t tracking_period_value) : tracking_period(tracking_period_value) {
        if (tracking_period == 0) {
            throw std::invalid_argument("expected value > 0");
        }
    };

    std::unique_ptr<StrategyOutput> proceed(const DOHLCV &data) override;

    std::string_view name() const override;

private:
    struct PriceEntry{
        double price;
        double price_change; // value > 0 is gain, value < 0 loss
    };
    const double lower = 30;
    const double upper = 80;
    std::list<PriceEntry> prices_history;
    double gains_sum = 0.0;
    double losses_sum = 0.0;
    uint8_t tracking_period = 14;
};
