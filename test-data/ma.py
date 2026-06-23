import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

base_path = Path(__file__).parent
df_strat = pd.read_csv(base_path / "strategy-output.csv")
df_wallet = pd.read_csv(base_path / "wallet-output.csv")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10), sharex=True,
                               gridspec_kw={'height_ratios': [2, 1]})

ax1.plot(df_strat.index, df_strat['Price'], label='Price', color='gray', alpha=0.3, linewidth=1)
ax1.plot(df_strat.index, df_strat['Fast_MA'], label='Fast MA', color='blue', linewidth=1.2)
ax1.plot(df_strat.index, df_strat['Slow_MA'], label='Slow MA', color='orange', linewidth=1.2)

completed_buys = df_strat[(df_strat['Signal'] == 'BUY') & (df_strat['Completed'] == 1)]
completed_sells = df_strat[(df_strat['Signal'] == 'SELL') & (df_strat['Completed'] == 1)]

incomplete_buys = df_strat[(df_strat['Signal'] == 'BUY') & (df_strat['Completed'] == 0)]
incomplete_sells = df_strat[(df_strat['Signal'] == 'SELL') & (df_strat['Completed'] == 0)]

ax1.scatter(completed_buys.index - 1 + completed_buys["T"], completed_buys['Transaction_Price'],
            marker='X', color='green', s=100, label='Completed BUY', zorder=5)
ax1.scatter(completed_sells.index - 1 + completed_sells["T"], completed_sells['Transaction_Price'],
            marker='X', color='red', s=100, label='Completed SELL', zorder=5)

ax1.scatter(incomplete_buys.index - 1 + incomplete_buys["T"], incomplete_buys['Transaction_Price'],
            marker='o', facecolors='none', edgecolors='green', s=100, label='Incomplete BUY', zorder=4)
ax1.scatter(incomplete_sells.index - 1 + incomplete_sells["T"], incomplete_sells['Transaction_Price'],
            marker='o', facecolors='none', edgecolors='red', s=100, label='Incomplete SELL', zorder=4)


ax1.set_title('MA Crossover Strategy & Equity Curve', fontsize=16)
ax1.set_ylabel('Price (USD)')
ax1.legend(loc='upper left')
ax1.grid(True, alpha=0.3)

ax2.plot(df_wallet.index, df_wallet['NetWorth'], label='Total Net Worth', color='purple', linewidth=2)
ax2.fill_between(df_wallet.index, df_wallet['NetWorth'], 10000,
                 where=(df_wallet['NetWorth'] >= 10000), color='green', alpha=0.1)
ax2.fill_between(df_wallet.index, df_wallet['NetWorth'], 10000,
                 where=(df_wallet['NetWorth'] < 10000), color='red', alpha=0.1)

ax2.set_ylabel('Equity (USD)')
ax2.set_xlabel('Tick / Time Index')
ax2.legend(loc='upper left')
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.show()