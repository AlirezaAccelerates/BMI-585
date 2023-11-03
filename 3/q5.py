import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('C:/Users/Alireza/Desktop/BMI 585/Assignments/3/pagerank_results.csv')

df = pd.DataFrame(data)

plt.plot(df['p'], df['speedup'], marker='o')
plt.xlabel('Number of nodes', fontsize =14)
plt.ylabel('Speed up', fontsize =14)
plt.title('Speed up vs different number of nodes', fontsize =15)
plt.grid()
plt.savefig('pr.png', dpi = 450)