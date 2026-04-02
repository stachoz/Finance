#include "CSVStrategyRecorder.h"

StrategyOutput CSVStrategyRecorder::proceed(const DOHLCV &data) {
    const StrategyOutput strategy_output = decorated_strategy->proceed(data);
    data_history.push_back(strategy_output);

    return strategy_output;
}

void CSVStrategyRecorder::save_to_file(const std::filesystem::path &filename,
    const std::vector<std::string> &header) const {
    std::ofstream file(filename.string());

    if (!header.empty()) {
        for (int i = 0; i < header.size() - 1; ++i) {
            file << header[i] << ",";
        }
        file << header[header.size() - 1] << "\n";
    }

    for (const auto& d : data_history) {
        file << d.signal << ","
                << d.fast_ma << ","
                << d.slow_ma << ","
                << d.price << ","
                << d.transaction_price << ","
                << d.t << "\n";
    }
}
