import matplotlib.pyplot as plt
import os

if __name__ == "__main__" :
    time = []
    norm = []
    f = open("out/plot.out", "r")
    lines = f.readlines()
    rdec = int(lines[0])
    rdecs = int(lines[1])
    for i in range(2, len(lines)):
        lines[i] = lines[i].strip('\n')
        data = lines[i].split(",")
        time.append(float(data[0]))
        norm.append(int(data[1]))
    f.close()

    plt.scatter(time,norm)
    plt.xlabel("Temps")
    plt.ylabel("Norme")
    plt.title("Evolution de la norme du chiffré")
    plt.plot(time, norm, label="Norme du chiffré")
    plt.axhline(y = rdec, color='r', label="rDec")
    plt.axhline(y = rdecs, color='green', label="rDec/(s+1)")
    plt.legend()
    plt.show()