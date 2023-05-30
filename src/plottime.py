import matplotlib.pyplot as plt
import math

if __name__ == "__main__" :
    steps = []
    time1 = []
    time2 = []
    f1 = open("time1.out", "r")
    f2 = open("time2.out", "r")
    lines1 = f1.readlines()
    lines2 = f2.readlines()
    step = 0
    for i in range(0, len(lines1)):
        lines1[i] = lines1[i].strip('\n')
        lines2[i] = lines2[i].strip('\n')
        steps.append("2^" + str(step + 5))
        step += 1
        time1.append(float(lines1[i]))
        time2.append(float(lines2[i]))
        
    for i in range(0, len(time1)) :
        print(time1[i])
        
    f1.close()
    f2.close()

    plt.scatter(steps,time1)
    plt.scatter(steps,time2)
    plt.xlabel("Dimension")
    plt.ylabel("Temps (s)")
    plt.title("Mesure du temps de génération des clés en fonction du temps")
    
    plt.plot(steps, time1, label="Génération sans multi-threading", color='r')
    plt.plot(steps, time2, label="Génération avec multi-threading", color='b')
    plt.legend()
    plt.show()
