import pandas as ps
import matplotlib.pyplot as plt
import numpy as np

def draw(l, subtitle, label, angleTHETA):
    temp = list(np.repeat(l, len(angleTHETA)))
    plt.style.use("ggplot")
    figure, ax = plt.subplots()
    figure.suptitle(subtitle)
    THETA_array = np.asarray(angleTHETA).T
    l_array = np.asarray(temp).T

    ax.plot(THETA_array, l_array, color="red", label=label, alpha=1, marker="s", markerfacecolor="black", markersize=1)

    ax.legend(loc="lower right")
    ax.set_xlabel("Theta angle, °")
    ax.set_ylabel(label)
    plt.show()


with open("THETAparams.txt", "r") as f:
    theta = f.readlines()
    f.close()

angleTHETA = theta[0].split(' ')
radTHETA = theta[0].split(' ')
angleTHETA.pop()
radTHETA.pop()
for i in range(len(angleTHETA)):
    angleTHETA[i] = int(angleTHETA[i])
    radTHETA[i] = float(int(radTHETA[i]) * np.pi / 180)

with open("unperturbed.txt", "r") as f:
    data = f.readlines()
    f.close()


with open("unperturbedRadius.txt", "r") as f:
    radius = f.readlines()
    f.close()

R = radius[0].split(' ')
R.pop()
for i in range(len(R)):
    R[i] = float(R[i])


DATA = data[0].split(' ')
DATA.pop()
P = float(DATA[0])
SIGMA = float(DATA[1])
INC = float(DATA[2])
OMEGA = float(DATA[3])
ECC = float(DATA[4])

chosenToDraw = list(np.repeat(OMEGA, 25))

#/--------------------------/
#Orbital trajectory
plt.figure()
plt.polar(radTHETA, R)
plt.title("Orbital trajectory of a space aircraft without perturbation")
plt.show()
#/--------------------------/

draw(P, "Focal Parameter, km", "P", angleTHETA)
draw(ECC, "Eccentricity", "ECC", angleTHETA)
draw(OMEGA, "Perigee argument, rad.", "ω", angleTHETA)
draw(INC, "Inclanation, rad.", "i", angleTHETA)
draw(SIGMA, "Longtitude of the ascending node, rad.", "Ω", angleTHETA)
