#pragma once

#include <chrono>
#include <iostream>
#include <fstream>

#include "CSVReader.hpp"
#include "Engine.h"
#include "TimeSeries.h"

namespace fs = std::filesystem;

class TradeSimApplication {
public:
    void start_app();

private:
    [[nodiscard]] TimeSeries create_time_series_from_csv() const;

    Engine engine;
    fs::path aapl_data = fs::path(PROJECT_ROOT_DIR) / "aapl_us_d.csv";
};
