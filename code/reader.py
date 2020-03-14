import matplotlib.pyplot as plt
import sys
import os




f = open("./data_All3_CombinedCartesian.txt")
a = 7
b= 0
values=[]
for line in f:
    b+=1
    if(b <a):
        continue
    value = float(line.split('\t')[2])-120
    values.append(value)

    

#print("Percentage of values bellow "+str(maximum)+" "+str(bellow/len(values))+"%")

bins_values = [-500,-120,-110,-100,-90,-80,-70,-60,300]

n, bins, patches = plt.hist(values, bins=bins_values, facecolor='g', alpha=0.75,histtype= 'step')
#plt.show()
plt.show()

x = [0,1,2,3,4,5,6,7]
plt.bar(x,n/len(values))
plt.title("Percentage of area per RX dbm")
plt.xticks(ticks=x,labels=["<0","0-10","10-20","20-30","30-40","40-50","50-60",">60"])
plt.show()

acc = 0

cum = [0]*len(n)
print(cum)
for i in range(len(n)):
    acc+=n[i]
    cum[i] = acc/len(values)



plt.bar(x,cum)
plt.title("prob cumulative")
plt.xticks(ticks=x,labels=["<0","0-10","10-20","20-30","30-40","40-50","50-60",">60"])
plt.show()


print(n[0]/len(values))
print((n[0]+n[1])/len(values))

