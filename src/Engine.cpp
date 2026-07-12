#include "Engine.h"
#include "Statisitcs.h"

void Engine::run() const {
    auto awaiting_task = Signal::NONE;

    for (const auto& daily_data : data) {
        // Signal is computed based on closing price.
        // Transaction is made with open price to simulate delay
        wallet->update(awaiting_task, daily_data.open);
        const auto strategy_output = strategy->proceed(daily_data);
        awaiting_task = strategy_output.signal;
    }

    std::cout << "Final stats: " << std::endl << wallet->get_string();
    std::cout << "return: " << *stats::returns(std::vector{
        wallet->get_initial_saldo(),
        wallet->get_total_net_worth()
    }).begin();
}

