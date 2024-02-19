import numpy as np

# 定义指数函数
def exponential_function(x, a, b):
    return a * np.power(2, b * x)

# 给定参数
a = 0.00058
b = 0.0024

# 特定的 x 值
x_values = [16384, 8192, 4096]

# 计算对应的 y 值
for x in x_values:
    y = exponential_function(x, a, b)
    print(f"When x is {x}, y is {y}")
