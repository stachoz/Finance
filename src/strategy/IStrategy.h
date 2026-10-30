#pragma once

#include <string>
#include <string_view>
#include <vector>
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

class StrategyOutput {
public:
    Signal signal;
    double price;                       /// Signal price
    double transaction_size;
    StrategyOutput(Signal signal, double price, double transaction_size) : signal(signal), price(price), transaction_size(transaction_size) {}

    virtual ~StrategyOutput() = default;

    virtual void write_strategy_as_row(std::ostream& os) const {
        os << signal << "," << price << "\n";
    }

    virtual const std::vector<std::string>& csv_header() const {
        static const std::vector<std::string> header{"signal", "price"};
        return header;
    }
};

class MAStrategyOutput : public StrategyOutput {
public:
    double fast_ma;
    double slow_ma;
    double t;

    MAStrategyOutput(const StrategyOutput& base, double fast_ma, double slow_ma, double t) :
        StrategyOutput{base}, fast_ma{fast_ma}, slow_ma{slow_ma}, t{t} {}

    void write_strategy_as_row(std::ostream& os) const override {
        os << signal << "," << price << "," << fast_ma << "," << slow_ma << "," << t << "\n";
    }

    const std::vector<std::string>& csv_header() const override {
        static const std::vector<std::string> header{"signal", "price", "fast_ma", "slow_ma", "t"};
        return header;
    }
};

struct RsiStrategyOutput : StrategyOutput {
    double rsi_value;
    RsiStrategyOutput(const StrategyOutput& base, double rsi_value)
        : StrategyOutput(base), rsi_value(rsi_value) {}

    void write_strategy_as_row(std::ostream& os) const override {
        os << signal << "," << price << "," << rsi_value << "\n";
    }

    const std::vector<std::string>& csv_header() const override {
        static const std::vector<std::string> header{"signal", "price", "rsi_value"};
        return header;
    }
};

class IStrategy {
public:
    virtual ~IStrategy() = default;
    virtual std::unique_ptr<StrategyOutput> proceed(const DOHLCV& data) = 0;
    virtual std::string_view name() const = 0;
};
