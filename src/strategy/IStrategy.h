#pragma once

#include <cstdint>
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
            stream << "UNKNOWN";
    }
    return stream;
}

struct StrategyOutput {
    Signal signal;
    double fast_ma;
    double slow_ma;
    double price;                       /// Signal price
    double t;                           /// Fractional day offset used to interpolate the precise timing of the crossover
    year_month_day date;                /// Date of closing price, based on which signal was caluclated
};

inline void write_strategy_as_row(std::ostream& os, const StrategyOutput &output) {
    os << output.signal << ","
        << output.fast_ma << ","
        << output.slow_ma << ","
        << output.price << ","
        << output.t << "\n";
}

class IStrategy {
public:
    virtual ~IStrategy() = default;
    virtual StrategyOutput proceed(const DOHLCV& data) = 0;
};