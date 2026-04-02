#include "Engine.h"

namespace fs = std::filesystem;

void Engine::run() const {
    const auto sliced_time_series =  time_series.slice({2025y, January, 1d});

    int buy = 0;
    int sell = 0;
    int none = 0;

    CSVStrategyRecorder strategy {std::make_unique<MAStrategy>(7, 21)};

    for (const auto& data : sliced_time_series) {
        const StrategyOutput strategy_output = strategy.proceed(data);

        switch (const auto signal = strategy_output.signal) {
            case Signal::BUY:
                buy++;
                break;
            case Signal::SELL:
                sell++;
                break;
            case Signal::NONE:
                none++;
                break;
        }
    }

    fs::path output_path = fs::path(PROJECT_ROOT_DIR) / "test-data" / "strategy-output.csv";

    strategy.save_to_file(output_path, {"Signal", "Fast_MA", "Slow_MA", "Price", "Transaction_Price", "T"});
}

void Engine::set_time_series(const TimeSeries &value) {
    time_series = value;
}
