#pragma once

#include <cstdint>
#include <iostream>

#include "TimeSeries.h"

enum class Signal : uint8_t {
    BUY,
    SELL,
    NONE
};

inline std::ostream& operator<<(std::ostream& stream, Signal signal) {
    switch (signal) {
        case Signal::BUY:
            stream << "BUY";
            break;
        case Signal::SELL:
            stream << "SELL";
            break;
        case Signal::NONE:
            stream << "NONE";
            break;
        default:
            stream << "BUY";
    }
    return stream;
}

struct StrategyOutput {
    Signal signal;
    double fast_ma;
    double slow_ma;
    double price;
    double transaction_price;
    double t;
};

class IStrategy {
public:
    virtual ~IStrategy() = default;
    virtual StrategyOutput proceed(const DOHLCV& data) = 0;
};