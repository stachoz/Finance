#include "TradeSimApplication.h"
#include "CSVExporter.h"

void TradeSimApplication::start_app() {
    const TimeSeries time_series {utils::csv::read(aapl_data, true)};
    auto sliced = time_series.slice({2022y, January, 1d});

    auto strategy_recorder = std::make_unique<StrategyRecorder>(std::make_unique<MAStrategy>(7, 21));
    auto wallet = std::make_unique<Wallet>(10'000, 30);

    const auto* recorder_view = strategy_recorder.get();
    const auto* wallet_view = wallet.get();

    engine = std::make_unique<Engine>(
        sliced,
        std::move(strategy_recorder),
        std::move(wallet)
    );

    engine->run();

    CSVExporter::export_strategy(*recorder_view, fs::path(TEST_DATA_DIR) / "strategy-output.csv");
    CSVExporter::export_wallet(*wallet_view, fs::path(TEST_DATA_DIR) / "wallet-output.csv");
}