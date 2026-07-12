#include "StrategyRecorder.h"


StrategyOutput StrategyRecorder::proceed(const DOHLCV &data) {
    const StrategyOutput strategy_output = decorated_strategy->proceed(data);
    data_history.push_back(strategy_output);

    return strategy_output;
}

const std::vector<StrategyOutput> & StrategyRecorder::get_data_history() const {
    return data_history;
}
