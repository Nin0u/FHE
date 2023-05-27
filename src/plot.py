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
        if(data[1] == "+") : colors.append('orange')
        if(data[1] == "r") : colors.append('m')
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
    plt.plot(color = 'c', label="Multiplications")
    plt.legend(["rDec", "rDec/(s+1)","Multiplcations", "Recrypt", "Petites Additions", "Grosses Additions"])
    plt.gca().get_legend().legendHandles[0].set_color('r')
    plt.gca().get_legend().legendHandles[1].set_color('g')
    plt.gca().get_legend().legendHandles[2].set_color('b')
    plt.gca().get_legend().legendHandles[3].set_color('m')
    plt.gca().get_legend().legendHandles[4].set_color('orange')
    plt.gca().get_legend().legendHandles[5].set_color('y')
    plt.show()
    #plt.savefig("graph.pdf")