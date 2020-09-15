#max cut in ising model

import numpy as np
from numpy.random import rand
import matplotlib.pyplot as plt
import time


niter=50    # Number of times we average over
N=100        # Problem Size
temp=10    # Temperature
nmoves=3000   # Number of MC moves to equilibriate
nmax=10   # Max number allowed

class Ising():

    def __init__(self,N,amp):
        self.N=N
        self.amp=amp
        self.E0=0
        self.E=0
        self.v=2*np.random.randint(0,2,N)-1
        j=np.where(self.v<0)
        self.amp[j,:]=-self.amp[j,:]
        self.amp[:,j]=-self.amp[:,j]
        self.cut=0
        self.initEnergy()      
        
    def initEnergy(self):
        #Calculates Initial Energy
        print(self.amp)
        ii=np.where(self.amp<0)
        self.cut=sum(self.amp[ii])/2
        print("Initial cut=",self.cut)
        

    def calcenergy(self,a):
        #Calculates difference in Energy if spin at a is flipped, i.e. the node a is transferred to the other set
        e=-sum(self.amp[a])
        return e

    def mcmove(self,beta):
        '''Makes N monte carlo moves'''
        for i in range(N):
            #print(self.amp)
            a = np.random.randint(0, self.N) #Picking Random node
            #print("Node {} selected".format(a))
            s=self.v[a]
            
            cost=self.calcenergy(a)

            #Flipping according to MC algorithm
            if cost < 0:	
                #print("Flip due to reduction in energy")
                s *= -1
                self.amp[a,:]=-self.amp[a,:]
                self.amp[:,a]=-self.amp[:,a]
                self.cut+=cost
            elif rand() < np.exp(-cost*beta):
                #print("Flip due to thermal energy")
                s *= -1
                self.amp[a,:]=-self.amp[a,:]
                self.amp[:,a]=-self.amp[:,a]
                self.cut+=cost

            self.v[a] = s

    def simulate(self,temp,nmoves):   
        '''Runs nmoves no. of iterations at temp'''
        """for i in range(nmoves):
            self.mcmove(1.0/temp)"""

        cutf=0
        i=0
        while(abs(cutf-self.cut)!=0): #Stop running when energy doesn't change. Other bounds can be used
            cutf=self.cut
            self.mcmove(1.0/temp)
            if(temp>0.1):temp=temp-0.001           ##attempt at annealing
            i+=1
        print(i)

"""l=np.random.randint(nmax,size=(N,N)) #Random Problem Instance
l=l+l.T
for i in range(N):l[i,i]=0;"""

data=''                                 #reading from a file
with open("input3.txt") as file:
    data=file.read()
l1=data.split("\n")[0]
n,m=l1.split()
n=int(n);m=int(m)

l=np.zeros(shape=(n,n))

for line in data.split("\n")[1:-1]:
    #print(line)
    line=line.strip()
    x,y,w=line.split()
    x=int(x)-1
    y=int(y)-1
    w=int(w)
    l[x,y]=w
    l[y,x]=w

rm=Ising(n,l)

t=0
cutn=0

for i in range(niter): #We do repeated iterations of the same instance and average over it
    ti=time.time()
    rm=Ising(n,l)
    rm.simulate(temp,nmoves)
    tf=time.time()
    t=t+tf-ti
    cutn=cutn+rm.cut
t=t/niter
cutn=cutn/niter
print(t)
print(cutn)
#print("Initial graph=\n",rm.amp)
"""rm.simulate(temp,nmoves)
print(rm.amp)
print("Final cut=",rm.cut)"""

"""fptr=open("different_temp.txt",'w')
tim=list()
ct=list()

for temp in np.arange(0.1,20.2,0.2):
    t=0     #Time taken
    cutn=0    #Final Energy 
    ti=0    #Dummy variables for time measurement
    tf=0
    for i in range(niter): #We do repeated iterations of the same instance and average over it
        ti=time.time()
        rm=Ising(n,l)
        rm.simulate(temp,nmoves)
        tf=time.time()
        t=t+tf-ti
        cutn=cutn+rm.cut
    t=t/niter
    cutn=cutn/niter    
    fptr.write("{} {} {}\n".format(temp,cutn,t))
    tim.append(t)
    ct.append(cutn)

plt.plot(ct)
plt.plot(tim)
plt.show()
fptr.close()"""