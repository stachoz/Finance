#include "TradeSimApplication.h"

void TradeSimApplication::start_app() {
    const auto time_series = create_time_series_from_csv();
    engine.set_time_series(time_series);
    engine.run();
}

TimeSeries TradeSimApplication::create_time_series_from_csv() const {
    const auto data = CSVReader::read(aapl_data);

    TimeSeries time_series;

    for(const auto& row : data) {
        std::chrono::year_month_day ymd {
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

        time_series.push_back(point);
    }

    return time_series;
}
