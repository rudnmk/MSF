import pandas as ps
import matplotlib.pyplot as plt
import numpy as np

with open("data_for_120.txt", "r") as f:
    ACC = f.readlines()
    f.close()

solar_activity = [75, 100, 125, 150, 175, 200, 250]
S_acc = []
T_acc = []
W_acc = []
OV_acc = []
for i in ACC:
    S_acc.append(i.split(" ")[0])
    T_acc.append(i.split(" ")[1])
    W_acc.append(i.split(" ")[2])
    OV_acc.append((i.split(" ")[3]).rstrip("\n"))


plt.style.use("ggplot")
figure, ax1 = plt.subplots()
figure2, ax2 = plt.subplots()
figure.suptitle("Graph of the components of the perturbing acceleration depending on the fixated level of solar activity on the first altitude range(120 - 500 km)")
figure2.suptitle("Graph of the perturbing acceleration depending on the fixated level of solar activity on the first altitude range(120 - 500 km)")

S_array = np.asarray(S_acc).T
T_array = np.asarray(T_acc).T
W_array = np.asarray(W_acc).T
OV_array = np.asarray(OV_acc).T

ax1.plot(solar_activity, S_array, color="red", label="S acceleration", alpha=1, marker="s", markerfacecolor="black", markersize=1)
ax1.plot(solar_activity, T_array, color="green", label="T acceleration", alpha=1, marker="s", markerfacecolor="black", markersize=1)
ax1.plot(solar_activity, W_array, color="blue", label="W acceleration", alpha=1, marker="s", markerfacecolor="black", markersize=1)

ax1.legend(loc="lower right")
ax1.set_xlabel("Level of solar activity F0, (10^-22) Wt/(m^2 * Hz)")
ax1.set_ylabel("Perturbing acceleration components, km/s^2")


ax2.plot(solar_activity, OV_array, color="black", label="Overall acceleration", alpha=1, marker="s", markerfacecolor="black", markersize=1)

ax2.legend(loc="lower right")
ax2.set_xlabel("Level of solar activity F0, (10^-22) Wt/(m^2 * Hz)")
ax2.set_ylabel("Perturbing acceleration, km/s^2")

plt.show()
