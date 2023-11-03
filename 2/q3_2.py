import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('C:/Users/Alireza/Desktop/BMI 585/Assignments/2/pi_results.csv')

df = pd.DataFrame(data)

for name, group in df.groupby('p'):
    plt.plot(group['n'], group['speedup'], "-o",label=f'{name} threads')
    plt.xscale('log', base=10)

plt.xlabel('Number of points (log scale)', fontsize =14)
plt.ylabel('Speed up', fontsize =14)
plt.title('Speed up vs number of points for different threads', fontsize =15)
plt.legend()
plt.savefig('sn.png', dpi = 450)