#include "Engine.h"

namespace fs = std::filesystem;

void Engine::run() const {
    const auto sliced_time_series = time_series.slice({2024y, January, 1d});

    CSVStrategyRecorder strategy {std::make_unique<MAStrategy>(7, 21)};
    const auto wallet = std::make_unique<Wallet>(10'000, 30);

    for (const auto& data : sliced_time_series) {
        const auto strategy_output = strategy.proceed(data);
        if (const bool was_successful = wallet->update(strategy_output.signal, strategy_output.price); !was_successful) {
            strategy.mark_last_as_not_completed();
        }
    }

    std::cout << "Final stats: " << std::endl << wallet->get_string();
    generate_stats_files(strategy, *wallet);
}

void Engine::generate_stats_files(const CSVStrategyRecorder& strategy, const Wallet &wallet) {
    const fs::path stats_file_path_dir = fs::path(PROJECT_ROOT_DIR) / "test-data";
    const fs::path output_path = stats_file_path_dir /  "strategy-output.csv";
    const fs::path wallet_path = stats_file_path_dir /  "wallet-output.csv";

    utils::csv::save_to_file(
        strategy.get_data_history(),
        output_path,
        write_strategy_as_row,
        {"Signal", "Fast_MA", "Slow_MA", "Price", "Transaction_Price", "T", "Completed"}
    );

    auto write_row = [](std::ostream& os, const std::pair<double, double>& row) {
        os << std::setprecision(5) << row.first << "," << row.second << "\n";
    };

    utils::csv::save_to_file<std::pair<double, double>>(
        wallet.get_wallet_history(),
        wallet_path,
        write_row,
        {"price", "NetWorth"}
    );
}

void Engine::set_time_series(const TimeSeries &value) {
    time_series = value;
}
