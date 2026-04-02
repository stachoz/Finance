from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

base_path = Path(__file__).parent

df = pd.read_csv(base_path / "strategy-output.csv")

plt.figure(figsize=(14, 7))

plt.plot(df.index, df['Price'], label='Price', color='gray', alpha=0.4, linewidth=1)
plt.plot(df.index, df['Fast_MA'], label='Fast MA', color='blue', linewidth=1.5)
plt.plot(df.index, df['Slow_MA'], label='Slow MA', color='orange', linewidth=1.5)

buys = df[df['Signal'] == 'BUY']
sells = df[df['Signal'] == 'SELL']

buys_x = buys.index - 1 + buys["T"]
sells_x = sells.index - 1 + sells["T"]

plt.scatter(buys_x, buys['Transaction_Price'], label='BUY Signal',
            marker='X', color='green', s=30, zorder=5)

plt.scatter(sells_x, sells['Transaction_Price'], label='SELL Signal',
            marker='X', color='red', s=30, zorder=5)

plt.title('MA Crossover Strategy Verification', fontsize=15)
plt.xlabel('Tick / Time Index')
plt.ylabel('Price')
plt.legend()
plt.grid(True, which='both', linestyle='--', alpha=0.5)

plt.show()