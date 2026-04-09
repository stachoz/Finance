#include "Engine.h"

#include "Wallet.h"

namespace fs = std::filesystem;

void Engine::run() const {
    const auto sliced_time_series = time_series.slice({2024y, January, 1d});

    CSVStrategyRecorder strategy {std::make_unique<MAStrategy>(7, 21)};
    const auto wallet = std::make_unique<Wallet>(10'000, 30);

    for (const auto& data : sliced_time_series) {
        const auto strategy_output = strategy.proceed(data);
        wallet->update(strategy_output.signal, strategy_output.price);
    }

    std::cout << "Final stats: " << std::endl << wallet->get_string();

    const fs::path output_path = fs::path(PROJECT_ROOT_DIR) / "test-data" / "strategy-output.csv";
    strategy.save_to_file(output_path, {"Signal", "Fast_MA", "Slow_MA", "Price", "Transaction_Price", "T"});
}

void Engine::set_time_series(const TimeSeries &value) {
    time_series = value;
}
