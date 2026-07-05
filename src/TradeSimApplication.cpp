#include "TradeSimApplication.h"

void TradeSimApplication::start_app() {
    const TimeSeries time_series {utils::csv::read(aapl_data, true)};
    engine.set_time_series(time_series);
    engine.run();
}
