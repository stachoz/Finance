#include "Engine.h"

void Engine::run() const {
    auto awaiting_task = Signal::NONE;
    double awaiting_size = 0;

    for (const auto& daily_data : data) {
        // Signal is computed based on closing price.
        // Transaction is made with open price to simulate delay
        wallet->update(awaiting_task, daily_data.open, awaiting_size);
        const auto strategy_output = strategy->proceed(daily_data);
        awaiting_task = strategy_output->signal;
        awaiting_size = strategy_output->transaction_size;
    }
}

const Wallet * Engine::get_wallet() const {
    return wallet.get();
}

