#pragma once

#include <filesystem>
#include <fstream>
#include <memory>

#include "IStrategy.h"
#include "File.hpp"

class StrategyRecorder : public IStrategy{
public:
    explicit StrategyRecorder(std::unique_ptr<IStrategy> strategy) : decorated_strategy(std::move(strategy)) {}

    StrategyOutput proceed(const DOHLCV &data) override;

    [[nodiscard]] const std::vector<StrategyOutput>& get_data_history() const;
private:
    std::unique_ptr<IStrategy> decorated_strategy;
    std::vector<StrategyOutput> data_history {};
};
