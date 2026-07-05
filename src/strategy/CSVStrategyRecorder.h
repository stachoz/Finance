#pragma once

#include <filesystem>
#include <fstream>
#include <memory>

#include "IStrategy.h"
#include "File.hpp"

class CSVStrategyRecorder : public IStrategy{
public:
    explicit CSVStrategyRecorder(std::unique_ptr<IStrategy> strategy) : decorated_strategy(std::move(strategy)) {}

    StrategyOutput proceed(const DOHLCV &data) override;

    [[nodiscard]] const std::vector<StrategyOutput>& get_data_history() const;
private:
    std::unique_ptr<IStrategy> decorated_strategy;
    std::vector<StrategyOutput> data_history {};
};
