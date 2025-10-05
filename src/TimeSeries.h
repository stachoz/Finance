#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <ranges>

struct DOHLCV {
    std::chrono::year_month_day date;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class TimeSeries {
public:
    TimeSeries(const std::vector<std::vector<std::string>>& data);

    void push_back(const DOHLCV& point);

    size_t size() const;

    auto begin() const;

    auto end() const;

    std::vector<double> closes() const;

    std::vector<DOHLCV> slice(std::chrono::year_month_day start_date, std::chrono::days duration) const;

    std::vector<DOHLCV> slice(std::chrono::year_month_day start_date, std::chrono::year_month_day end_date) const;

    template <typename T>
    static std::vector<double> values_of(T DOHLCV::*member, const std::vector<DOHLCV>& data) {
        std::vector<double> result;

        for(const auto& item : data) {
            result.push_back(item.*member);
        }

        return result;
    }

private:
    std::vector<DOHLCV> slice_impl(std::chrono::sys_days start_date,
                              std::chrono::sys_days end_date) const;

    std::vector<DOHLCV> values {};
};