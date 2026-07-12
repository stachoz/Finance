#include "Engine.h"
#include "File.hpp"
#include "TimeSeries.h"
#include "Wallet.h"
#include "strategy/MAStrategy.h"

int main() {
    const TimeSeries time_series {utils::csv::read(std::filesystem::path(PROJECT_ROOT_DIR) / "aapl_us_d.csv", true)};
    auto sliced = time_series.slice({2022y, January, 1d});

    std::make_unique<Wallet>(10'000, 30);

    auto engine = std::make_unique<Engine>(
        sliced,
        std::make_unique<MAStrategy>(7, 21),
        std::make_unique<Wallet>(10'000, 30)
    );

    engine->run();
}
