#number partitioning in ising model

import numpy as np
from numpy.random import rand
import matplotlib.pyplot as plt
import time
niter=50    # Number of times we average over
#N=1000     # Problem Size
temp=30     # Temperature
nmoves=50   # Number of MC moves to equilibriate
nmax=1000   # Max number allowed
class Ising():

    def __init__(self,N,amp):
        self.N=N
        self.lattice = 2*np.random.randint(2,size=(N,N))-1 #Spins
        self.lattice.resize(N,N)
        self.amp=amp	#Problem Instance values
        self.E0=0		#Initial Energy
        self.E=0		#Instantaneous Energy (During Iterations)
        self.sums=0; self.nmin=np.min(amp)		#Sum of spins*amplitudes #Finding the min number for bound 


    def initEnergy(self):
        #'''Calculates Initial Energy'''
        e=np.sum(self.lattice*self.amp)
        self.sums=e
        self.E0=e*e
        self.E=e*e

    def calcenergy(self,a,b):
        #Calculates difference in Energy if spin at (a,b) is flipped 
        e=-4*self.amp[a][b]*self.lattice[a][b]*(self.sums-(self.amp[a][b]*self.lattice[a][b]))
        return e

    def mcmove(self,beta):
        '''Makes N*N monte carlo moves'''
        for i in range(N):
            for j in range(N):
                a = np.random.randint(0, self.N) #Picking Random Lattice sites
                b = np.random.randint(0, self.N)
                s =  self.lattice[a,b]
                cost=self.calcenergy(a,b)
                #Flipping according to MC algorithm
                if cost < 0:	
                    s *= -1
                    self.sums=self.sums-2*self.amp[a][b]*self.lattice[a][b]
                    self.E+=cost
                elif rand() < np.exp(-cost*beta):
                    s *= -1
                    self.sums=self.sums-2*self.amp[a][b]*self.lattice[a][b]
                    self.E+=cost
                self.lattice[a,b] = s

    def simulate(self,temp,nmoves):   
        '''Runs nmoves no. of iterations at temp'''
        Ef=0
        # for i in range(nmoves):
        # 	self.mcmove(1.0/temp)
        while(abs(self.E-Ef)!=0): #Stop running when energy doesn't change. Other bounds can be used
            if self.E==0:
                break
            Ef=self.E
            self.mcmove(1.0/temp)

fptr=open("energies.txt",'w')
for N in range(50,55):
    l=np.random.randint(nmax,size=(N,N)) #Problem Instance
    t=0     #Time taken
    En=0    #Final Energy 
    ti=0    #Dummy variables for time measurement
    tf=0
    for i in range(niter): #We do repeated iterations of the same instance and average over it
        ti=time.time()
        rm=Ising(N,l)
        rm.initEnergy()
        rm.simulate(temp,nmoves)
        tf=time.time()
        t=t+tf-ti
        En=En+rm.E
    t=t/niter
    En=En/niter    
    fptr.write("{} {} {}\n".format(N,En,t))
fptr.close()