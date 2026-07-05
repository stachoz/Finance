import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

base_path = Path(__file__).parent
df_strat = pd.read_csv(base_path / "strategy-output.csv")
df_wallet = pd.read_csv(base_path / "wallet-output.csv")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10), sharex=True,
                               gridspec_kw={'height_ratios': [2, 1]})

ax1.plot(df_wallet.index, df_wallet['market_price'], label='Price', color='gray', alpha=0.3, linewidth=1)
ax1.plot(df_strat.index, df_strat['fast_ma'], label='Fast MA', color='blue', linewidth=1.2)
ax1.plot(df_strat.index, df_strat['slow_ma'], label='Slow MA', color='orange', linewidth=1.2)

buys = df_strat[df_strat['signal'] == 'BUY']
sells = df_strat[df_strat['signal'] == 'SELL']

ax1.scatter(buys.index - 1 + buys["t"], buys['price'],
            marker='^', color='limegreen', s=100, label='BUY Signal', zorder=4, edgecolors='black')
ax1.scatter(sells.index - 1 + sells["t"], sells['price'],
            marker='v', color='crimson', s=100, label='SELL Signal', zorder=4, edgecolors='black')

ax1.set_title('MA Crossover Strategy & Equity Curve', fontsize=16)
ax1.set_ylabel('Price (USD)')
ax1.legend(loc='upper left')
ax1.grid(True, alpha=0.3)

ax2.plot(df_wallet.index, df_wallet['net_worth'], label='Total Net Worth', color='purple', linewidth=2)

# Fill between to highlight profit (green) and drawdown (red) assuming 10000 starting capital
ax2.fill_between(df_wallet.index, df_wallet['net_worth'], 10000,
                 where=(df_wallet['net_worth'] >= 10000), color='green', alpha=0.1)
ax2.fill_between(df_wallet.index, df_wallet['net_worth'], 10000,
                 where=(df_wallet['net_worth'] < 10000), color='red', alpha=0.1)

ax2.set_ylabel('Equity (USD)')
ax2.set_xlabel('Tick / Time Index')
ax2.legend(loc='upper left')
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.show()