import scipy
from scipy.stats import binom
import matplotlib.pyplot as plt
n = 15
users = 25
# setting the values
# of n and p
# defining list of r values
r_values = list(range(1,n+1))
plt.figure(2, figsize=(15,10))
plt.xticks(r_values)
plt.figure(1)
plt.xticks(range(1,users+1))
# list of pmf values
for id in range(1,users+1):
    p = id/users
    dist = [binom.pmf(r, n, p) for r in r_values]
    # plotting the graph
    mean = binom.mean(n,p)
    print(mean)
    plt.figure(1)
    plt.scatter(id,mean, label = "mean of id = "+str(id))
    plt.figure(2)
    plt.plot(r_values, dist, label = "p = "+str(p), marker = "o")
plt.show()
