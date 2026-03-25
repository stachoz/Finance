#include "TimeSeries.h"

TimeSeries::TimeSeries(const std::vector<std::vector<std::string>> &data) {
    for(const auto& row : data) {
        const year_month_day ymd {
            year(std::stoi(row[0].substr(0, 4))),
            month(std::stoi(row[0].substr(5, 2))),
            day(std::stoi(row[0].substr(8, 2)))
        };

        DOHLCV point {
            ymd,
            std::stod(row[1]),
            std::stod(row[2]),
            std::stod(row[3]),
            std::stod(row[4]),
            std::stod(row[5])
        };

        push_back(point);
    }
}

void TimeSeries::push_back(const DOHLCV &point) {
    values.push_back(point);
}

size_t TimeSeries::size() const {
    return values.size();
}

std::span<const DOHLCV> TimeSeries::slice(year_month_day start_date, std::optional<year_month_day> end_date) const {
    return slice_impl(start_date, end_date.value_or(year::max() / December / 31d));
}

std::span<const DOHLCV> TimeSeries::slice_impl(year_month_day start_date, year_month_day end_date) const {
    auto begin = std::ranges::lower_bound(values, start_date, {}, &DOHLCV::date);
    auto end = std::ranges::lower_bound(values, end_date, {}, &DOHLCV::date);

    if (begin > end) {
        return {};
    }

    return {begin, end};
}
