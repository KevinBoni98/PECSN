import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.stats import t
import json

# Load data from JSON file
with open('stats.json', 'r') as file:
    data = json.load(file)
k = list(data.keys())
rows = 50
avgs = [[] for _ in range(rows)]
for j in range (0,10):
    d = data.get(k[j])
    for i in range (0,50):
        avgs[i].append(d.get('statistics')[i].get('mean'))
avgs = np.asarray(avgs)
mean_values = np.mean(avgs, axis=1)
print(mean_values)
confidence_interval = 1.96 * np.std(avgs, axis=1) / np.sqrt(avgs.shape[1])

# Create a time axis (replace this with your actual time points)
time_points = np.arange(len(mean_values))

# Plotting the results
sns.set(style="darkgrid")  # Set the style for seaborn
plt.figure(figsize=(10, 6))
plt.errorbar(time_points, mean_values, yerr=confidence_interval, fmt='o', markersize=8, capsize=5, label='Mean')
plt.fill_between(time_points, mean_values - confidence_interval, mean_values + confidence_interval, alpha=0.2,
                 label='Confidence Interval')
plt.xlabel('Users')
plt.ylabel('Average Value')
plt.title('Average with Confidence Interval over 10 Simulations')
plt.legend()
plt.show()
'''
# Extract time and value vectors for each repetition
time_vectors = [rep_data['vectors'][0]['time'] for rep_data in data.values()]
value_vectors = [rep_data['vectors'][0]['value'] for rep_data in data.values()]

# Flatten the value vectors
flattened_data = np.array(value_vectors).flatten()

# Set the confidence level and degrees of freedom
confidence_level = 0.95
degrees_of_freedom = len(time_vectors[0]) - 1

# Calculate the critical value from the t-distribution
critical_value = t.ppf((1 + confidence_level) / 2, degrees_of_freedom)

# Calculate the mean and confidence interval
mean_value = np.mean(flattened_data)
confidence_interval = critical_value * np.std(flattened_data) / np.sqrt(len(flattened_data))

# Plotting the results
sns.set(style="darkgrid")  # Set the style for seaborn
plt.figure(figsize=(10, 6))
plt.plot([1, len(flattened_data)], [mean_value, mean_value], color='r', label='Overall Mean')
plt.fill_between([1, len(flattened_data)], mean_value - confidence_interval, mean_value + confidence_interval,
                 color='g', alpha=0.2, label='Confidence Interval')
plt.xlabel('Data Points')
plt.ylabel('Value')
plt.title('Overall Average with Confidence Interval')
plt.legend()
plt.show()
'''