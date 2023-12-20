import numpy as np
import matplotlib.pyplot as plt


def sliding_window_average(data, window_size):
    """Calculate the sliding window average."""
    cumsum = np.cumsum(data, dtype=float)
    cumsum[window_size:] = cumsum[window_size:] - cumsum[:-window_size]
    return cumsum[window_size - 1:] / window_size


def main():
    # Generate some random data
    np.random.seed(42)
    data = np.random.rand(100)
    print(data)
    # Set the window size for the sliding average
    window_size = 5

    # Calculate the sliding window average
    smoothed_data = sliding_window_average(data, window_size)

    # Plot the original data and the sliding window average
    plt.plot(data, label='Original Data', marker='o')
    plt.plot(smoothed_data, label=f'Sliding Window Average (Window Size={window_size})', linewidth=2)

    # Add labels and legend
    plt.xlabel('Data Point')
    plt.ylabel('Value')
    plt.title('Line Chart with Sliding Window Average')
    plt.legend()

    # Show the plot
    plt.show()


if __name__ == "__main__":
    main()
