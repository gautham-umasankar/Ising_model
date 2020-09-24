import sys
import numpy as np

n = 4
N = 4*4

if(len(sys.argv) > 1):
    n = (int(sys.argv[1]))
    N = n**2

opt = input("Choose type:s,m,t,r:\n")
if(opt == 's'):
    #Square lattice (10x10), all anti ferromagnetic
    J = np.zeros((N,N))
    print("J - Square lattice")
    for i in range(n):
        for j in range(n-1):
            J[n*i+j, n*i+j+1] = 1
            J[n*i+j+1, n*i+j] = 1
            J[i+n*j, i+n*(j+1)] = 1
            J[i+n*(j+1), i+n*j] = 1
    print(J)

elif(opt == 'm'):
    #Mobius lattice (10) in a circle, all anti ferromagnetic
    J = np.zeros((N,N))
    print("J - Mobius")
    for i in range(N):
        J[i, (i+1)%N] = 1
        J[(i+1)%N, i] = 1
        J[i, (i+N//2)%N] = 1
        J[(i+N//2)%N, i] = 1
    print(J)

elif(opt == "t"):
    #Triangular lattice (10), all anti ferromagnetic
    J = np.zeros((N,N))
    for i in range(n):
        for j in range(n-1):
            J[n*i+j, n*i+j+1] = 1
            J[n*i+j+1, n*i+j] = 1
            J[i+n*j, i+n*(j+1)] = 1
            J[i+n*(j+1), i+n*j] = 1
            if((i+1+n*(j+1))<N):
                J[i+n*j, i+1+n*(j+1)] = 1
                J[i+n*(j+1), i+1+n*j] = 1
    print("J - Triangular")
    print(J)

elif(opt == "r"):
    #Square lattice, random ferro/anti ferro
    print("J4 - Random square")
    J = np.zeros((N,N))
    for i in range(n):
        for j in range(n-1):
            choice1 = np.random.choice([1,-1],1)
            choice2 = np.random.choice([1,-1],1)
            J[n*i+j, n*i+j+1] = choice1
            J[n*i+j+1, n*i+j] = choice1
            J[i+n*j, i+n*(j+1)] = choice2
            J[i+n*(j+1), i+n*j] = choice2
    print(J)

f = open("./../Maxcut_instances/"+opt+"_"+str(N)+".txt", "w")
f.write(str(N))
f.write("\n")
for i in range(N):
    for j in range(i+1,N):
        if(J[i,j]!=0):
            f.write(str(i+1)+" "+str(j+1)+" "+str(int(J[i,j]))+"\n")
f.close()