#include "StrategyRecorder.h"


std::unique_ptr<StrategyOutput> StrategyRecorder::proceed(const DOHLCV &data) {
    auto strategy_output = decorated_strategy->proceed(data);
    auto result = strategy_output->signal;
    data_history.push_back(std::move(strategy_output));

    if (const auto* ma = dynamic_cast<const MAStrategyOutput*>(data_history.back().get())) {
        return std::make_unique<MAStrategyOutput>(*ma, ma->fast_ma, ma->slow_ma, ma->t);
    }
    if (const auto* rsi = dynamic_cast<const RsiStrategyOutput*>(data_history.back().get())) {
        return std::make_unique<RsiStrategyOutput>(*rsi, rsi->rsi_value);
    }
    return std::make_unique<StrategyOutput>(*data_history.back());
}

const std::vector<std::unique_ptr<StrategyOutput>> & StrategyRecorder::get_data_history() const {
    return data_history;
}

std::string_view StrategyRecorder::name() const {
    return decorated_strategy->name();
}
