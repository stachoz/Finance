#pragma once
#include <vector>
#include <chrono>

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
    void push_back(const DOHLCV& point) {
        values.push_back(point);
    }

    size_t size() const {
        return values.size();
    }

    auto begin() const {
        return values.begin();
    }

    auto end() const {
        return values.end();
    }

    std::vector<double> closes() const {
        std::vector<double> result;
        result.reserve(values.size());

        for(const auto& v : values) {
            result.push_back(v.close);
        }

        return result;
    }

private:
    std::vector<DOHLCV> values {};
};