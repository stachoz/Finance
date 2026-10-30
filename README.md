# TradeSim

This project is my personal sandbox for learning how stock exchanges work and improving my C++ skills. I’m building a backtester for strategies based on popular technical indicators, with the long-term hope of experimenting with an investing bot in live markets.

## Data flow

1. `FinanceSim` reads historical daily OHLCV data from `aapl_us_d.csv` into a `TimeSeries` and selects the period to simulate.
2. The `Engine` feeds each day’s data to a strategy, such as moving-average crossover or RSI. It sends each resulting signal to the wallet on the next day’s open, while signals are calculated from closing prices.
3. The wallet tracks cash, shares, transaction costs, and net worth. The app exports strategy details and wallet history as CSV files under `test-data/`.
4. `test-data/plot_strategy.py` reads those CSV exports and plots prices, indicator values, signals, and wallet value.

## Plotting results

After generating the CSV exports, run the plotting script from the project root:

```bash
python3 test-data/plot_strategy.py
```

If both strategy exports are present, choose one explicitly:

```bash
python3 test-data/plot_strategy.py test-data/rsi-output.csv
```

Use `ma-output.csv` instead to plot the moving-average strategy. The script requires Python with `pandas` and `matplotlib` installed.
