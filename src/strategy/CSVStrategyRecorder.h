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

    const std::vector<StrategyOutput>& get_data_history() const;

    void mark_last_as_not_completed();

private:
    std::unique_ptr<IStrategy> decorated_strategy;
    std::vector<StrategyOutput> data_history {};
};
