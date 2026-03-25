#pragma once

#include <algorithm>
#include <vector>
#include <chrono>
#include <iostream>
#include <ranges>

using namespace std::chrono;

struct DOHLCV {
    year_month_day date;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class TimeSeries {
public:
    explicit TimeSeries(const std::vector<std::vector<std::string>>& data);

    TimeSeries() = default;

    void push_back(const DOHLCV& point);

    size_t size() const;

    auto begin() const {
        return values.begin();
    }

    auto end() const {
        return values.end();
    }

    std::span<const DOHLCV> slice(year_month_day start_date, std::optional<year_month_day> end_date = std::nullopt) const;


private:
    std::vector<DOHLCV> values {};

    /// Note: potential performance issues due to year_month_day comparisons
    /// @param start_date
    /// @param end_date
    /// @return
    std::span<const DOHLCV> slice_impl(year_month_day start_date, year_month_day end_date) const;
};