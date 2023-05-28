import talib
import pandas as pd
import numpy as np

print(talib)

# 创建一个随机价格序列
prices = np.random.rand(100) * 100

# 使用 pandas 库将价格序列转换为 DataFrame
df = pd.DataFrame(prices, columns=["Close"])

# 计算 20 天的简单移动平均线（SMA）
sma20 = talib.SMA(prices, timeperiod=20)

# 将 SMA 数据添加到 DataFrame 中
df["SMA20"] = sma20

# 打印结果
print(df)
