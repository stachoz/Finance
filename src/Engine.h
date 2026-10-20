#pragma once

#include "TimeSeries.h"
#include "Wallet.h"

class Engine {
public:
    Engine(std::span<const DOHLCV> data, std::unique_ptr<IStrategy> strategy, std::unique_ptr<Wallet> wallet)
        : data(data),
            strategy(std::move(strategy)),
            wallet(std::move(wallet)) {}

    void run() const;

    const Wallet* get_wallet() const;

private:
    std::span<const DOHLCV> data;
    std::unique_ptr<IStrategy> strategy;
    std::unique_ptr<Wallet> wallet;
};
