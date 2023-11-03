import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('C:/Users/Alireza/Desktop/BMI 585/Assignments/3/quicksort_results.csv')

df = pd.DataFrame(data)

plt.plot(df['p'], df['speedup'], marker='o', label = 'quicksort with log')
plt.plot(df['p'], df['speedup_log'], marker='s', label = 'quicksort without log')
plt.xlabel('Number of nodes', fontsize =14)
plt.ylabel('Speed up', fontsize =14)
plt.title('Speed up vs different number of nodes', fontsize =15)
plt.grid()
plt.legend()
plt.savefig('qs.png', dpi = 450)