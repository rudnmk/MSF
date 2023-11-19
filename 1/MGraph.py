import pandas as ps
import matplotlib.pyplot as plt
import numpy as np


with open("M_Data.txt", "r") as f:
    a = f.readlines()
    f.close()


data_list = []
for i in a:
    data_list.append(float(i.rstrip('\n')))

plt.style.use("ggplot")
figure = plt.figure()
surface = figure.add_subplot()

result_array = np.asarray(data_list).T
surface.plot(range(10695), result_array, color="red", alpha=0.3, label="M", marker="s", markerfacecolor="black", markersize=1)
plt.legend(loc="upper left")
surface.set_xlabel("Time(t)")
surface.set_ylabel("Average Anomaly(M)")
plt.show()
