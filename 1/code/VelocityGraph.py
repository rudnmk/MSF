import pandas as ps
import matplotlib.pyplot as plt
import numpy as np


with open("T.txt", "r") as f:
    T = int(f.readline().split(".")[0])
    f.close()

with open("Velocity_Data.txt", "r") as f:
    a = f.readlines()
    f.close()


data_list = []
for i in a:
    data_list.append(float(i.rstrip('\n')))

plt.style.use("ggplot")
figure = plt.figure()
surface = figure.add_subplot()

result_array = np.asarray(data_list).T
surface.plot(range(T), result_array, color="red", alpha=0.3, label="V", marker="s", markerfacecolor="black", markersize=1)
plt.legend(loc="upper left")
surface.set_xlabel("Time(t), сек.")
surface.set_ylabel("Velocity(V), км/сек")
plt.show()
