import matplotlib.pyplot as plt



# # Little core LFU
# prefetchers = ["None", "Both Stride", "Both IndirectMemory","Both SignaturePath","Both Tagged","L2 Stride + L1D IndiredcMemory","L2 Stride + L1D SignaturePath",
#                "L2 Stride + L1D Tagged","L2 IndirectMemory + L1D Stride", "L2 SignaturePath + L1D Stride","L2 Tagged + L1D Stride"]
# l2_miss_rates = [0.000786, 0.000544, 0.000493, 0.000184, 0.000105, 0.000666, 0.000427, 0.000216, 0.000393, 0.000401, 0.000305]
# l1d_miss_rates = [0.491623, 0.365556, 0.253988, 0.439313, 0.492828, 0.254087, 0.351883, 0.492853, 0.366714, 0.367933, 0.366522]


# # Big core LFU
# prefetchers = ["None", "Both Stride", "Both IndirectMemory","Both SignaturePath","Both Tagged","L2 Stride + L1D IndiredcMemory","L2 Stride + L1D SignaturePath",
#                "L2 Stride + L1D Tagged","L2 IndirectMemory + L1D Stride", "L2 SignaturePath + L1D Stride","L2 Tagged + L1D Stride"]
# l2_miss_rates = [0.000785, 0.000730, 0.000435, 0.000460, 0.000315, 0.000723, 0.000749, 0.000683, 0.000426, 0.000412, 0.000300]
# l1d_miss_rates = [0.493330, 0.492506, 0.490857, 0.491998, 0.492048, 0.491023, 0.492342, 0.492461, 0.492483, 0.492469, 0.492568]


# Big core LRU
prefetchers = ["None", "Both Stride", "Both IndirectMemory","Both SignaturePath","Both Tagged","L2 Stride + L1D IndiredcMemory","L2 Stride + L1D SignaturePath",
               "L2 Stride + L1D Tagged","L2 IndirectMemory + L1D Stride", "L2 SignaturePath + L1D Stride","L2 Tagged + L1D Stride"]
l2_miss_rates = [0.000764, 0.000724, 0.000426, 0.000448, 0.000329, 0.000736, 0.000739, 0.000676, 0.000413, 0.000403, 0.000289]
l1d_miss_rates = [0.506047, 0.505651, 0.505588, 0.505661, 0.505605, 0.505617, 0.505814, 0.506028, 0.505557, 0.505524, 0.505534]


# Normalize the data
l2_max = max(l2_miss_rates)
l2_min = min(l2_miss_rates)
l1d_max = max(l1d_miss_rates)
l1d_min = min(l1d_miss_rates)

l2_miss_rates_normalized = [(x - l2_min) / (l2_max - l2_min) for x in l2_miss_rates]
l1d_miss_rates_normalized = [(x - l1d_min) / (l1d_max - l1d_min) for x in l1d_miss_rates]

# Plot the data
plt.figure(figsize=(10, 6))

# Plot normalized data
plt.plot(prefetchers, l1d_miss_rates_normalized, marker='o', label="L1d Miss Rate (Normalized)")
plt.plot(prefetchers, l2_miss_rates_normalized, marker='s', label="L2 Miss Rate (Normalized)")

# Plot original data
plt.plot(prefetchers, l1d_miss_rates, linestyle='--', marker='o', color='gray', label="L1d Miss Rate (Original)")
plt.plot(prefetchers, l2_miss_rates, linestyle='--', marker='s', color='gray', label="L2 Miss Rate (Original)")

# Add labels with relative changes
for i in range(len(prefetchers)):
    plt.text(i, l1d_miss_rates_normalized[i], f"{l1d_miss_rates_normalized[i]*100:.2f}%", ha='right', va='bottom', fontsize=9, color='blue')
    plt.text(i, l2_miss_rates_normalized[i], f"{l2_miss_rates_normalized[i]*100:.2f}%", ha='right', va='top', fontsize=9, color='orange')

# Set plot title and labels
plt.title("Normalized Cache Prefetcher Performance", fontsize=14)
plt.xlabel("Cache Configuration", fontsize=12)
plt.ylabel("Normalized Miss Rate", fontsize=12)
plt.xticks(rotation=45, ha='right', fontsize=10)
plt.yticks(fontsize=10)
plt.legend(fontsize=10)

# Remove grid
plt.grid(False)

# Show plot
plt.tight_layout()
plt.show()
