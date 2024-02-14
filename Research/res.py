import matplotlib.pyplot as plt

# # LRU 16X16
# prefetchers = ["None", "L1 Stride", "L1 IndirectMemory","L1 SignaturePath","L1 Tagged","L1I Stride + L1D IndiredcMemory","L1I Stride + L1D SignaturePath",
#                "L1I Stride + L1D Tagged","L1I IndirectMemory + L1D Stride", "L1I SignaturePath + L1D Stride","L1I Tagged + L1D Stride"]
# l1i_miss_rates = [0.084, 0.085, 0.085, 0.056, 0.056, 0.085, 0.085, 0.086, 0.085, 0.055, 0.053]
# l1d_miss_rates = [0.203, 0.174, 0.187, 0.171, 0.156, 0.187, 0.172, 0.156, 0.174, 0.174, 0.177]

# # Random 16X16
# prefetchers = ["None", "L1 Stride", "L1 IndirectMemory","L1 SignaturePath","L1 Tagged","L1I Stride + L1D IndiredcMemory","L1I Stride + L1D SignaturePath",
#                "L1I Stride + L1D Tagged","L1I IndirectMemory + L1D Stride", "L1I SignaturePath + L1D Stride","L1I Tagged + L1D Stride"]
# l1i_miss_rates = [0.085, 0.086, 0.086, 0.056, 0.057, 0.086, 0.087, 0.088, 0.086, 0.057, 0.053]
# l1d_miss_rates = [0.203, 0.174, 0.183, 0.171, 0.155, 0.183, 0.172, 0.157, 0.174, 0.174, 0.179]


# # LRU  64X64
# prefetchers = ["None", "L1 Stride", "L1 IndirectMemory","L1 SignaturePath","L1 Tagged","L1I Stride + L1D IndiredcMemory","L1I Stride + L1D SignaturePath",
#                "L1I Stride + L1D Tagged","L1I IndirectMemory + L1D Stride", "L1I SignaturePath + L1D Stride","L1I Tagged + L1D Stride"]
# l1i_miss_rates = [0.0070, 0.0070, 0.0070, 0.0048, 0.0046, 0.0070, 0.0070, 0.0070, 0.0070, 0.0047, 0.0045]
# l1d_miss_rates = [0.1083, 0.0724, 0.0906, 0.0859, 0.0966, 0.0906, 0.0860, 0.0979, 0.0724, 0.0705, 0.0712]


# Random  64X64
prefetchers = ["None", "L1 Stride", "L1 IndirectMemory","L1 SignaturePath","L1 Tagged","L1I Stride + L1D IndiredcMemory","L1I Stride + L1D SignaturePath",
               "L1I Stride + L1D Tagged","L1I IndirectMemory + L1D Stride", "L1I SignaturePath + L1D Stride","L1I Tagged + L1D Stride"]
l1i_miss_rates = [0.0070, 0.0072, 0.0070, 0.0049, 0.0048, 0.0070, 0.0070, 0.0070, 0.0072, 0.0049, 0.0048]
l1d_miss_rates = [0.1002, 0.0716, 0.0815, 0.0859, 0.0918, 0.0815, 0.0850, 0.0932, 0.0668, 0.0675, 0.0674]


# 
plt.figure(figsize=(10, 6))
plt.plot(prefetchers, l1d_miss_rates, marker='o',label="L1d Miss Rate")
plt.plot(prefetchers, l1i_miss_rates, marker='s',label="L1I Miss Rate")

# 
plt.title("Cache Prefetcher Performance")
plt.xlabel("Cache Prefetcher")
plt.ylabel("Miss Rate")
plt.legend()

# 
plt.show()
