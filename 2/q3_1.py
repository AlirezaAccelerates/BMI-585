import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('C:/Users/Alireza/Desktop/BMI 585/Assignments/2/pi_results.csv')

df = pd.DataFrame(data)

filtered_df = df[df['n'] == 200000000]

plt.plot(filtered_df['p'], filtered_df['speedup'], marker='o')
plt.xlabel('Number of threads', fontsize =14)
plt.ylabel('Speed up', fontsize =14)
plt.title('Speed up vs different number of threads', fontsize =15)
plt.savefig('sd.png', dpi = 450)