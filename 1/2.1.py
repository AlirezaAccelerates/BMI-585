import matplotlib.pyplot as plt
import numpy as np

# Given values
f = 0.30

# Core counts P from 1 to 1000 (using nearest powers of 2)
P_values = [2**i for i in range(0, 11)] 

# Amdahl's Law (Strong Scaling)
def amdahl_speedup(P):
    return 1 / (f + (1 - f) / P)

strong_scaling = [amdahl_speedup(P) for P in P_values]

# Gustafson's Law (Weak Scaling)
def gustafson_speedup(P):
    return P - f * (P - 1)

weak_scaling = [gustafson_speedup(P) for P in P_values]

# Plot
plt.figure(figsize=(10, 6))
plt.plot(P_values, strong_scaling, '-o', label="Strong Scaling (Amdahl's Law)")
plt.plot(P_values, weak_scaling, '-s', label="Weak Scaling (Gustafson's Law)")
plt.xscale('log', base=10)
plt.yscale('log', base=10)
plt.xlabel('Number of Processors (P)', fontsize = 14)
plt.ylabel('Speedup (S)', fontsize = 14)
plt.title('Strong vs. Weak Scaling Behavior', fontsize = 16)
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()
