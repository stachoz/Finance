#pragma once

#include "strategy/MAStrategy.h"
#include "TimeSeries.h"
#include "strategy/CSVStrategyRecorder.h"

class Engine {
public:
    void run() const;

    void set_time_series(const TimeSeries& value);

private:
    TimeSeries time_series;
};
