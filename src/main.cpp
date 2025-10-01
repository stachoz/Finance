#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "CSVReader.hpp"
#include "Statisitcs.h"
#include "TimeSeries.h"


void print(auto&& container) {
    std::ranges::for_each(container, [&](auto&& item) {
        std::cout << item << std::endl;;
    });
    std::cout << std::endl;
}

int main() {
    std::string aapl_data = "/home/stachoz/Projects/Finance/aapl_us_d.csv";

    auto data = CSVReader::read(aapl_data);

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

    std::cout << stats::mean(time_series.closes());
    std::cout << stats::covariance<int>({1,2,3}, {3,2,1});

    print(stats::moving_average<int>({1,2,3,4,5}, 2));

    return 0;
}
