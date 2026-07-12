#include "CSVExporter.h"

void CSVExporter::export_strategy(const StrategyRecorder &strategy_recorder, const fs::path &output_path) {
    utils::csv::save_to_file(
        strategy_recorder.get_data_history(),
        output_path,
        write_strategy_as_row,
        {"signal", "fast_ma", "slow_ma", "price","t"}
    );
}
void CSVExporter::export_wallet(const Wallet& wallet, const fs::path& output_path) {
    auto write_row = [](std::ostream& os, const std::pair<double, double>& row) {
        os << std::setprecision(5) << row.first << "," << row.second << "\n";
    };

    utils::csv::save_to_file<std::pair<double, double>>(
        wallet.get_wallet_history(),
        output_path,
        write_row,
        {"market_price", "net_worth"}
    );
}
