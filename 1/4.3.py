import pandas as pd
import matplotlib.pyplot as plt

# Load the data
data = pd.read_csv('error_results.txt')

# Plot
plt.figure(figsize=(10,6))
plt.plot(data['Time'], data['Error'], '-o', label='Computed Error')
plt.xlabel('Time (micro seconds)')
plt.ylabel('Error (%)')
plt.title('Time to Compute vs. Computed Error')
plt.legend()
plt.grid(True)
plt.show()
