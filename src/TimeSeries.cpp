#include "TimeSeries.h"

TimeSeries::TimeSeries(const std::vector<std::vector<std::string>> &data) {
    for(const auto& row : data) {
        const std::chrono::year_month_day ymd {
            std::chrono::year(std::stoi(row[0].substr(0, 4))),
            std::chrono::month(std::stoi(row[0].substr(5, 2))),
            std::chrono::day(std::stoi(row[0].substr(8, 2)))
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

auto TimeSeries::begin() const {
    return values.begin();
}

auto TimeSeries::end() const {
    return values.end();
}

std::vector<double> TimeSeries::closes() const {
    std::vector<double> result;
    result.reserve(values.size());

    for(const auto& v : values) {
        result.push_back(v.close);
    }

    return result;
}

std::vector<DOHLCV> TimeSeries::slice(std::chrono::year_month_day start_date, std::chrono::days duration) const {
    using namespace std::chrono;

    const sys_days start = start_date;
    const sys_days end = start + duration;
    return slice_impl(start, end);
}

std::vector<DOHLCV> TimeSeries::slice(std::chrono::year_month_day start_date,
    std::chrono::year_month_day end_date) const {
    using namespace std::chrono;

    return slice_impl(sys_days(start_date), sys_days(end_date));
}

std::vector<DOHLCV> TimeSeries::slice_impl(std::chrono::sys_days start_date, std::chrono::sys_days end_date) const {
    auto view = values |
                std::views::filter([&](const DOHLCV& d) {
                    return d.date > start_date && d.date < end_date;
                });

    return {view.begin(), view.end()};
}
