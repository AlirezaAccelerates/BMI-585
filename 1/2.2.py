import matplotlib.pyplot as plt
import numpy as np

# Given values
f1 = 0.30
f2 = 0.30
f3 = 0.40

# Speedup for strong scaling
def strong_speedup(P):
    P_node = P / 4
    return 1 / (f1 + f2 / P_node + f3 / P)

# Speedup for weak scaling
def weak_speedup(P):
    return P - f1 * (P - 1) - f2 * (P / 4 - 1)

# Core counts P from 1 to 1000 (using nearest powers of 2)
P_values_extended = [2**i for i in range(0, 11)] 

strong_scaling_extended = [strong_speedup(P) for P in P_values_extended]
weak_scaling_extended = [weak_speedup(P) for P in P_values_extended]

# Plot
plt.figure(figsize=(10, 6))
plt.plot(P_values_extended, strong_scaling_extended, '-o', label="Strong Scaling")
plt.plot(P_values_extended, weak_scaling_extended, '-s', label="Weak Scaling")
plt.xscale('log', base=10)
plt.yscale('log', base=10)
plt.xlabel('Number of Processors (P)', fontsize = 14)
plt.ylabel('Speedup (S)', fontsize = 14)
plt.title('Strong vs. Weak Scaling Behavior', fontsize = 16)
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()
