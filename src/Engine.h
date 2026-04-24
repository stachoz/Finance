#pragma once

#include "strategy/MAStrategy.h"
#include "TimeSeries.h"
#include "strategy/CSVStrategyRecorder.h"
#include "Wallet.h"

class Engine {
public:
    void run() const;

    void set_time_series(const TimeSeries& value);

private:
    static void generate_stats_files(const CSVStrategyRecorder &strategy, const Wallet &wallet);

    TimeSeries time_series;
};
