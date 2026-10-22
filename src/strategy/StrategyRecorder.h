#pragma once

#include <filesystem>
#include <fstream>
#include <memory>

#include "IStrategy.h"
#include "File.hpp"

class StrategyRecorder : public IStrategy{
public:
    explicit StrategyRecorder(std::unique_ptr<IStrategy> strategy) : decorated_strategy(std::move(strategy)) {}

    std::unique_ptr<StrategyOutput> proceed(const DOHLCV &data) override;
    std::string_view name() const override;

    [[nodiscard]] const std::vector<std::unique_ptr<StrategyOutput>>& get_data_history() const;
private:
    std::unique_ptr<IStrategy> decorated_strategy;
    std::vector<std::unique_ptr<StrategyOutput>> data_history {};
};
