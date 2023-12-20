import json
import numpy as np
import pandas as pd

import matplotlib.pyplot as plt
#uniformCQI-240-20231215-10:35:53-18296
# Load data from the JSON file
with open('warmupUni.json', 'r') as file:
    data = json.load(file)

'''
def sliding_window_average_2d(data, window_size):
    """Calculate the sliding window average for a 2D array."""
    result = np.zeros_like(data, dtype=float)

    for i in range(data.shape[0] - window_size + 1):
        result[i:i + window_size, :] += data[i:i + window_size, :]

    result[:window_size - 1, :] /= np.arange(1, window_size)
    result[window_size - 1:, :] /= window_size

    return result
'''


win = 1000
#print(data['uniformCQI-240-20231215-10:35:53-18296']['vectors'][0].get('time'))
# Extract values for 'time' and 'value' attributes

#data = np.convolve(value_values, np.ones(win) / win, mode='same')
#sli = sliding_window_average_2d(data,5)
colors = ['red','blue','orange','purple','green','yellow','black','pink','teal','brown']
# Plot the values in a line chart
i = 0
keys = list(data.keys())
plt.title('Total # of packets in queues over time (sliding window average)')
plt.xlabel('Time (s)')
plt.ylabel('Packets')

for x in range(0,9):
    time_values = data[keys[x]]['vectors']
    value_values = data[keys[x]]['vectors']
    xv = time_values[0].get('time')
    yv = value_values[0].get('value')
    series = pd.Series(yv)
    rolled = series.rolling(window=win, min_periods=100).mean()
    #dataa = np.convolve(yv, np.ones(win) / win, mode='same')
    plt.figure(1)
    plt.plot(xv, rolled, label='Packets received per timeslot', color=colors[i])
    plt.figure(2)
    plt.plot(xv, rolled, label='Packets received per timeslot', color=colors[i])
    i+=1
zoom_start = 0
zoom_end = 10
#

#plt.legend()
plt.figure(2)
plt.xticks(np.arange(min(xv), max(xv) + 1, 1))
plt.xlim(zoom_start, zoom_end)
plt.show()