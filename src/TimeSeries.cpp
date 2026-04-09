#include "TimeSeries.h"

TimeSeries::TimeSeries(const std::vector<DOHLCV> &data) {
    values = data;
}

void TimeSeries::push_back(const DOHLCV &point) {
    values.push_back(point);
}

size_t TimeSeries::size() const {
    return values.size();
}

void TimeSeries::reserve(size_t size) {
    values.reserve(size);
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
