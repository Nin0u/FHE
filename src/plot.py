import matplotlib.pyplot as plt
import os

if __name__ == "__main__" :
    print("Plotting the results ...", end="")
    time = []
    norm = []
    f = open("out/plot.out", "r")
    lines = f.readlines()
    rdec = int(lines[0])
    for i in range(1, len(lines)):
        data = lines[i].split(",")
        time.append[data[0]]
        norm.append[data[1]]
    f.close()

    plt.xlabel("Temps")
    plt.ylabel("Norme")
    plt.title("Evolution de la norme du chiffré")
    plt.plot(time, norm, label="Norme du chiffré")
    plt.axhline(y = rdec, color='r', label="rayon de déchiffrement")
    plt.legend()
    plt.show()
    print("Done.")
