
import numpy as np
import matplotlib.pyplot as plt

# 
peak_flops = 512  # Peak flops/s
dram_bandwidth = 73  # DRAM bandwidth (GB/s)
ai = 0.118  # Program Arithmetic Intensity

# x
x = np.linspace(0, 1, 100)

# 
plt.plot(x, [peak_flops]*len(x), label='Peak Gflops/s', linestyle='--', color='b', linewidth=2)
plt.plot(x, dram_bandwidth * x, label='Dram Bandwidth', linestyle='--', color='r', linewidth=2)
# plt.plot(x, [2.0]*len(x), label='Version 3 Gflops/s:0.262', linestyle='-', color='g', linewidth=2)
# plt.plot(x, [2.95]*len(x), label='Version 4 Gflops/s:0.295', linestyle='-', color='m', linewidth=2)
plt.plot([ai, ai], [0, peak_flops], linestyle='-', color='k', linewidth=2)

# 
plt.text(ai + 0.02, 0, f'AI = {ai}', verticalalignment='bottom')
plt.text(ai + 0.01, peak_flops, f'{peak_flops} GFLOPs/s', verticalalignment='top')
plt.text(ai + 0.02, ai * dram_bandwidth, f'{ai * dram_bandwidth:.2f} GFLOPs/s', verticalalignment='top')

# 
plt.xlabel('Arithmetic Intensity (FLOPs/Byte)')
plt.ylabel('Performance (GFLOPs/s)')

# 
plt.legend()

# 
plt.grid(True)  # 
plt.tight_layout()  # 
plt.show()