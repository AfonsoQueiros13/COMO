import matplotlib.pyplot as plt
import sys
import os


if len(sys.argv) != 3:
    print("""
    Usage: python3 code.py /path/to/data/file MAXIMUM
    MAXIMUM: The program will output the percentage of values bellow the given maximum value
    """)
    sys.exit(0)

data_file_lines = open(sys.argv[1],'r').readlines()
values = []
maximum = int(sys.argv[2])
bellow = 0 

for line in data_file_lines[6:]:
	value = float(line.split('\t')[2])-120
    values.append(value)
    if value < maximum:
        bellow += 1

print("Percentage of values bellow "+str(maximum)+" "+str(bellow/len(values))+"%")


bins_values = [-500,-120,-110,-100,-90,-80,-70,-60,300]

n, bins, patches = plt.hist(values, bins=bins_values, facecolor='g', alpha=0.75,histtype= 'step')
#plt.show()
plt.show()
x = [0,1,2,3,4,5,6,7]

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




