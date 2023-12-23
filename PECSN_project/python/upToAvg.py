import json
import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

# uniformCQI-240-20231215-10:35:53-18296
# Load data from the JSON file
with open('pInQ_U50_L25.json', 'r') as file:
    data = json.load(file)

def calculate_averages(up_to_index, arr):
    if up_to_index > 0 and up_to_index <= 7000:
        averages = []
        for a in range(1, up_to_index + 1):
            b_values = arr[:a]  # Select values from 0 to a-1
            average_in_point_a = np.sum(b_values) / a
            averages.append(average_in_point_a)
            print(a)
        return averages
    else:
        return None  # Handle invalid values of up_to_index


# Example usage
up_to_index = 7000
colors = ['red', 'blue', 'orange', 'purple', 'green', 'yellow', 'black', 'pink', 'teal', 'brown']
# Plot the values in a line chart
i = 0
keys = list(data.keys())
#plt.title('Total # of packets in queues over time (sliding window average)')
plt.xlabel('Time (s)')
plt.ylabel('Packets')

for x in range(0, 10):
    time_values = data[keys[x]]['vectors']
    value_values = data[keys[x]]['vectors']
    xv = time_values[0].get('time')
    yv = value_values[0].get('value')
    avgy = calculate_averages(up_to_index, yv)
    plt.figure(1)
    plt.xticks(np.arange(0, 71, step=5))
    plt.plot(xv[:up_to_index], avgy, label='Packets received per timeslot', color=colors[i])
    plt.figure(2)
    plt.plot(xv[:up_to_index], avgy, label='Packets received per timeslot', color=colors[i])
    i += 1
zoom_start = -0.5
zoom_end = 5
#

# plt.legend()
plt.figure(2)
plt.xticks(np.arange(min(xv), max(xv) + 1, 1))
plt.xlim(zoom_start, zoom_end)
plt.show()