import pandas as ps
import matplotlib.pyplot as plt
import numpy as np


def auto(l):
    temp = l[0].split(' ')
    temp.pop()
    for i in range(len(temp)):
        temp[i] = float(temp[i])
    return temp

def draw(l, subtitle, label, angleTHETA):
    plt.style.use("ggplot")
    figure, ax = plt.subplots()
    figure.suptitle(subtitle)
    THETA_array = np.asarray(angleTHETA).T
    l_array = np.asarray(l).T

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

with open("radius.txt", "r") as f:
    radius = f.readlines()
    f.close()
with open("p.txt", "r") as f:
    p = f.readlines()
    f.close()
with open("ecc.txt", "r") as f:
    ecc = f.readlines()
    f.close()
with open("omega.txt", "r") as f:
    omega = f.readlines()
    f.close()
with open("i.txt", "r") as f:
    i = f.readlines()
    f.close()
with open("sigma.txt", "r") as f:
    sigma = f.readlines()
    f.close()
with open("S.txt", "r") as f:
    s = f.readlines()
    f.close()
with open("T.txt", "r") as f:
    t = f.readlines()
    f.close()
with open("W.txt", "r") as f:
    w = f.readlines()
    f.close()

P = auto(p)
ECC = auto(ecc)
OMEGA = auto(omega)
INC = auto(i)
SIGMA = auto(sigma)
S = auto(s)
T = auto(t)
W = auto(w)
R = auto(radius)
print(max(T), min(T))

draw(P, "Focal Parameter, km", "P", angleTHETA)
draw(ECC, "Eccentricity", "ECC", angleTHETA)
draw(OMEGA, "Perigee argument, rad.", "ω", angleTHETA)
draw(INC, "Inclanation, rad.", "i", angleTHETA)
draw(SIGMA, "Longtitude of the ascending node, rad.", "Ω", angleTHETA)
draw(S, "S acceleration, km/h^2", "S", angleTHETA)
draw(T, "T acceleration, km/h^2", "T", angleTHETA)
draw(W, "W acceleration, km/h^2", "W", angleTHETA)

#/--------------------------/
#Orbital trajectory
plt.figure()
plt.polar(radTHETA, R)
plt.title("Orbital trajectory of a space aircraft with perturbation")
plt.show()
#/--------------------------/
