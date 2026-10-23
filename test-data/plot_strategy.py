"""Plot the strategy and wallet CSV exports from FinanceSim.

The strategy is selected from the strategy CSV filename (ma-*.csv or rsi-*.csv).
Pass that CSV path as the first argument, or let the script find a recognized
strategy output file alongside itself.
"""

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


BASE_PATH = Path(__file__).parent
KNOWN_STRATEGY_FILES = {
    "ma-output.csv": "ma",
    "rsi-output.csv": "rsi",
}
WALLET_FILES = {
    "ma": "ma-wallet.csv",
    "rsi": "rsi-wallet.csv",
}


def find_strategy_file():
    candidates = [BASE_PATH / filename for filename in KNOWN_STRATEGY_FILES]
    existing = [path for path in candidates if path.is_file()]
    if not existing:
        raise SystemExit(
            "No recognized strategy CSV found next to this script. "
            "Expected ma-output.csv or rsi-output.csv."
        )
    if len(existing) > 1:
        names = ", ".join(path.name for path in existing)
        raise SystemExit(
            f"Found multiple strategy CSV files ({names}); pass one as an argument."
        )
    return existing[0]


def strategy_from_filename(path):
    strategy = KNOWN_STRATEGY_FILES.get(path.name.lower())
    if strategy is None:
        known = " or ".join(KNOWN_STRATEGY_FILES)
        raise SystemExit(f"Unknown strategy file {path.name!r}; expected {known}.")
    return strategy


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "strategy_csv",
        nargs="?",
        type=Path,
        help="MA or RSI strategy output CSV (inferred from its filename)",
    )
    args = parser.parse_args()

    if args.strategy_csv:
        strategy_path = args.strategy_csv
        strategy = strategy_from_filename(strategy_path)
        if not strategy_path.is_absolute() and not strategy_path.is_file():
            strategy_path = BASE_PATH / strategy_path
        if not strategy_path.is_file():
            raise SystemExit(f"Strategy CSV does not exist: {strategy_path}")
    else:
        strategy_path = find_strategy_file()
        strategy = strategy_from_filename(strategy_path)

    wallet_path = BASE_PATH / WALLET_FILES[strategy]
    if not wallet_path.is_file():
        raise SystemExit(
            f"Wallet CSV for the {strategy.upper()} strategy is missing: {wallet_path.name}"
        )

    df_strat = pd.read_csv(strategy_path)
    df_wallet = pd.read_csv(wallet_path)

    if strategy == "rsi":
        fig, (ax1, ax2, ax3) = plt.subplots(
            3, 1, figsize=(14, 12), sharex=True,
            gridspec_kw={"height_ratios": [2, 1, 1]},
        )
    else:
        fig, (ax1, ax2) = plt.subplots(
            2, 1, figsize=(14, 10), sharex=True,
            gridspec_kw={"height_ratios": [2, 1]},
        )

    ax1.plot(
        df_wallet.index, df_wallet["market_price"], label="Price",
        color="gray", alpha=0.3, linewidth=1,
    )

    if strategy == "ma":
        ax1.plot(df_strat.index, df_strat["fast_ma"], label="Fast MA", color="blue", linewidth=1.2)
        ax1.plot(df_strat.index, df_strat["slow_ma"], label="Slow MA", color="orange", linewidth=1.2)
        ax1.set_title("MA Crossover Strategy & Equity Curve", fontsize=16)
    else:
        ax2.plot(df_strat.index, df_strat["rsi_value"], label="RSI", color="teal", linewidth=1.2)
        ax2.axhline(30, color="gray", linestyle="--", linewidth=1, label="Lower threshold (30)")
        ax2.axhline(80, color="darkorange", linestyle="--", linewidth=1, label="Upper threshold (80)")
        ax2.set_ylim(0, 100)
        ax1.set_title("RSI Strategy & Indicator", fontsize=16)

    buys = df_strat[df_strat["signal"] == "BUY"]
    sells = df_strat[df_strat["signal"] == "SELL"]
    buy_x = buys.index - 1 + buys["t"] if strategy == "ma" else buys.index
    sell_x = sells.index - 1 + sells["t"] if strategy == "ma" else sells.index
    ax1.scatter(buy_x, buys["price"], marker="^", color="limegreen", s=100,
                label="BUY Signal", zorder=4, edgecolors="black")
    ax1.scatter(sell_x, sells["price"], marker="v", color="crimson", s=100,
                label="SELL Signal", zorder=4, edgecolors="black")

    ax1.set_ylabel("Price (USD)")
    ax1.legend(loc="upper left")
    ax1.grid(True, alpha=0.3)

    if strategy == "ma":
        ax2.plot(df_wallet.index, df_wallet["net_worth"], label="Total Net Worth", color="purple", linewidth=2)
        ax2.fill_between(df_wallet.index, df_wallet["net_worth"], 10000,
                         where=(df_wallet["net_worth"] >= 10000), color="green", alpha=0.1)
        ax2.fill_between(df_wallet.index, df_wallet["net_worth"], 10000,
                         where=(df_wallet["net_worth"] < 10000), color="red", alpha=0.1)
        ax2.set_ylabel("Equity (USD)")
    else:
        ax2.set_ylabel("RSI")
        ax3.plot(df_wallet.index, df_wallet["net_worth"], label="Total Net Worth", color="purple", linewidth=2)
        ax3.fill_between(df_wallet.index, df_wallet["net_worth"], 10000,
                         where=(df_wallet["net_worth"] >= 10000), color="green", alpha=0.1)
        ax3.fill_between(df_wallet.index, df_wallet["net_worth"], 10000,
                         where=(df_wallet["net_worth"] < 10000), color="red", alpha=0.1)
        ax3.set_ylabel("Equity (USD)")
        ax3.set_xlabel("Tick / Time Index")
        ax3.legend(loc="upper left")
        ax3.grid(True, alpha=0.3)

    if strategy == "ma":
        ax2.set_xlabel("Tick / Time Index")
    ax2.legend(loc="upper left")
    ax2.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
