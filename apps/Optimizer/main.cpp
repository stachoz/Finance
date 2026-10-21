#include "Engine.h"
#include "File.hpp"
#include "TimeSeries.h"
#include "Wallet.h"
#include "strategy/ma/MAStrategy.h"

int main() {
    const TimeSeries time_series {utils::csv::read(std::filesystem::path(PROJECT_ROOT_DIR) / "aapl_us_d.csv", true)};
    auto sliced = time_series.slice({2020y, January, 1d});

    const std::vector fast_ma_values {5, 8, 13, 21, 34};
    const std::vector slow_ma_values {21, 34, 55, 89, 233};

    std::pair<int, int> best_ma_values {0, 0};
    double best_saldo = 0;

    for (auto fast_value : fast_ma_values) {
        for (auto slow_value : slow_ma_values) {
            if (fast_value > slow_value) {
                continue;
            }
            const auto engine = std::make_unique<Engine>(
                sliced,
                std::make_unique<MAStrategy>(fast_value, slow_value),
                std::make_unique<Wallet>(10'000, 30)
            );

            engine->run();

            if (const double final_net_worth = engine->get_wallet()->get_last_total_net_worth(); final_net_worth > best_saldo) {
                best_ma_values = {fast_value, slow_value};
                best_saldo = final_net_worth;
            }
        }
    }

    std::cout << "best saldo: " << best_saldo << std::endl;
    std::cout << "best ma values (slow, fast): " << best_ma_values.first << ", " << best_ma_values.second << std::endl;
}
