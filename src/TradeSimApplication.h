#pragma once

#include "File.hpp"
#include "Engine.h"
#include "TimeSeries.h"

namespace fs = std::filesystem;

class TradeSimApplication {
public:
    void start_app();

private:
    std::unique_ptr<Engine> engine;
    fs::path aapl_data = fs::path(PROJECT_ROOT_DIR) / "aapl_us_d.csv";
};
