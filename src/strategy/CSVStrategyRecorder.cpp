#include "CSVStrategyRecorder.h"


StrategyOutput CSVStrategyRecorder::proceed(const DOHLCV &data) {
    const StrategyOutput strategy_output = decorated_strategy->proceed(data);
    data_history.push_back(strategy_output);

    return strategy_output;
}

void CSVStrategyRecorder::save_to_file(const std::filesystem::path &filename,
    const std::vector<std::string> &header) const {
    utils::csv::save_to_file(
        data_history,
        filename,
        header);
}
