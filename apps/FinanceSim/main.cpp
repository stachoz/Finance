#include "CSVExporter.h"
#include "Engine.h"
#include "File.hpp"
#include "TimeSeries.h"
#include "Wallet.h"
#include "strategy/ma/MAStrategy.h"
#include "strategy/StrategyRecorder.h"
#include "strategy/rsi/RsiStrategy.h"

int main() {
    const TimeSeries time_series {utils::csv::read(fs::path(PROJECT_ROOT_DIR) / "aapl_us_d.csv", true)};
    auto sliced = time_series.slice({2020y, January, 1d});

    constexpr double initial_saldo = 10'000;
    constexpr double shares_per_transaction = 30;

    auto strategy_recorder = std::make_unique<StrategyRecorder>(std::make_unique<RsiStrategy>(14));
    auto wallet = std::make_unique<Wallet>(initial_saldo, shares_per_transaction);

    const auto* recorder_view = strategy_recorder.get();
    const auto* wallet_view = wallet.get();

    const auto engine = std::make_unique<Engine>(
        sliced,
        std::move(strategy_recorder),
        std::move(wallet)
    );

    engine->run();

    std::cout << wallet_view->get_string();

    CSVExporter::export_strategy(*recorder_view, fs::path(TEST_DATA_DIR) / "strategy-output.csv");
    CSVExporter::export_wallet(*wallet_view, fs::path(TEST_DATA_DIR) / "wallet-output.csv");
}
