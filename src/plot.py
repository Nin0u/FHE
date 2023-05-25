import matplotlib.pyplot as plt
import math

if __name__ == "__main__" :
    steps = []
    colors = []
    norm = []
    f = open("out/plot.out", "r")
    lines = f.readlines()
    rdec = math.log10(int(lines[0]))
    rdecs = math.log10(int(lines[1]))
    step = 0
    for i in range(2, len(lines)):
        lines[i] = lines[i].strip('\n')
        data = lines[i].split(" ")
        steps.append(step)
        step += 1
        norm.append(math.log10(int(data[0])))
        if(data[1] == "*") : colors.append('b')
        if(data[1] == "+") : colors.append('g')
        if(data[1] == "r") : colors.append('r')
        if(data[1] == "!") : colors.append('y')
    f.close()

    #plt.scatter(steps,norm)
    plt.xlabel("Etapes")
    plt.ylabel("Norme")
    plt.title("Evolution de la norme du chiffré")
    for i in range(0, len(norm) - 1) :
        plt.plot([steps[i], steps[i + 1]], [norm[i], norm[i + 1]], color=colors[i])
    #plt.plot(steps, norm, label="Norme du chiffré")
    plt.axhline(y = rdec, color='r', label="rDec")
    plt.axhline(y = rdecs, color='green', label="rDec/(s+1)")
    plt.legend()
    plt.show()