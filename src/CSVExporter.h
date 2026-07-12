#pragma once
#include "Wallet.h"
#include "strategy/StrategyRecorder.h"

namespace fs = std::filesystem;

class CSVExporter {
public:
    static void export_strategy(const StrategyRecorder& strategy_recorder, const fs::path& output_path);

    static void export_wallet(const Wallet& wallet, const fs::path& output_path);
};
