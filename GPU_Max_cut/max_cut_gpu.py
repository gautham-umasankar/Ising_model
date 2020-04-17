from numba import cuda
from numpy import *
from numpy import random

""" Max_Cut matrix is given by J_i,j"""

nmax = 10
N = 3
beta=0.1

threadsperblock = N
blockspergrid = 1

nmax = 10
N = 3
beta=0.1

@cuda.jit
def flip():
    i=cuda.threadIdx.x%N;
    e = -sum(J[i])
    if(e<0):
        print("Flipping deterministically",i)
        J[i,:] = -J[i,:]
        J[:,i] = -J[:,i]
    elif (random.rand()<exp(-e*beta)):
        print("Flipping randomly",i)
        J[i,:] = -J[i,:]
        J[:,i] = -J[:,i]
    print(J)


# J=random.randint(nmax,size=(N,N)) #Random Problem Instance
# J=J+J.T
# for i in range(N):J[i,i]=0;

J=array([[0,-1,2],[-1,0,3],[2,3,0]])  
print(J)
flip[blockspergrid,threadsperblock]()
print(J)




    




    