#include "CSVExporter.h"

void CSVExporter::export_strategy(const StrategyRecorder &strategy_recorder, const fs::path &output_path) {
    const auto& history = strategy_recorder.get_data_history();

    if (history.empty()) {
        utils::csv::save_to_file(
            history,
            output_path,
            &StrategyOutput::write_strategy_as_row
        );
    }
    else {
        utils::csv::save_to_file(
            history,
            output_path,
            &StrategyOutput::write_strategy_as_row,
            history.front()->csv_header()
        );
    }
}
void CSVExporter::export_wallet(const Wallet& wallet, const fs::path& output_path) {
    auto write_row = []( const std::pair<double, double>& row, std::ostream& os) {
        os << std::setprecision(5) << row.first << "," << row.second << "\n";
    };

    utils::csv::save_to_file<std::pair<double, double>>(
        wallet.get_wallet_history(),
        output_path,
        write_row,
        {"market_price", "net_worth"}
    );
}
