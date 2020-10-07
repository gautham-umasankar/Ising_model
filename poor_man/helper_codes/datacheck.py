import numpy as np 
import matplotlib.pyplot as plt
import sys

data_file = ""
N = 10
J_file = 0
if(len(sys.argv)>1):
    i = 1
    while(i <= len(sys.argv[1:])):
        opt = str(sys.argv[i])
        #print(opt)
        #File name
        if(opt == '-data'):
            data_file = str(sys.argv[i+1])
        elif(opt == '-N'):
            N = int(sys.argv[i+1])
        elif(opt == '-j'):
            J_file = str(sys.argv[i+1])    
        i+=1
        

if(J_file):
    f = open(J_file,"r")
    N, number_of_edges = [int(i) for i in f.readline().split()]
    J = np.zeros([N,N])
    lines = f.readlines()
    for line in lines:
        l = line.split()
        r,c,w = int(l[0])-1, int(l[1])-1, float(l[2])
        J[r][c] = w
        J[c][r] = w        
    f.close()
    #print(J)

def cut_value(cut):
    value = 0
    for row,i in enumerate(cut):
        for col,j in enumerate(cut):
            if(i*j<0):
                #print(i,j)
                #print("Row and column",row,col,J[row][col])
                value = value + J[row][col]
    return value/2


f = open(data_file)
lines = f.readlines()
comment = lines[0][1:]
f.close()

lines = lines[3:]
N = int(len(lines[0].split()[4:]))
traj = np.zeros([N,1])
N_iter = len(lines)

for line in lines:
    traj = np.c_[traj,np.reshape(np.array([float(i) for i in line.split()[4:]]),[N,1])]
traj = traj[:,1:]


check = np.zeros(traj.shape)

for i,spin in enumerate(traj):
    check[i] = np.array(list([spin[0]]) + (list(spin[0:N_iter-1]*spin[1:])))

cut_values = np.array([cut_value(traj[:,i]) for i in range(N_iter)])
counts = np.array([len(np.where(check[:,i]>0)[0]) for i in range(N_iter-1)])


count_threshold = 0.3*N
value_threshold = 0.3

count_pass = len(np.where(counts[N_iter-6:]> count_threshold))
#print(np.where(abs(traj[:,N_iter-6:])>value_threshold))

if(count_pass<=6 or len(np.where(abs(traj[:,N_iter-6:])>value_threshold)[0])):
    print("Fail")

plot1 = plt.figure()
for i in range(N):
        plt.plot(traj[i],"-",label = "spin {}".format(i))
plt.title("N_spins = {} ".format(N) + "Trajectory " + comment)
plt.grid(True)

plot2 = plt.figure()
for i in range(N):
        plt.plot(check[i],"-",label = "spin {}".format(i))
plt.title("check")
plt.grid(True)

plot3 = plt.figure()
plt.plot(counts,label = "Counts")
plt.plot(cut_values,label = "Cut_values")
plt.legend()
plt.title("Count")
plt.grid(True)

plot4 = plt.figure()
plt.plot(cut_values)
plt.title("Cut_values")
plt.grid(True)

plt.show()