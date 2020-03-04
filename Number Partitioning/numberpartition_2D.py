#number partitioning in ising model

import numpy as np
from numpy.random import rand
import matplotlib.pyplot as plt
import time

N=3         # Problem Size
temp=30     # Temperature
nmoves=50   # Number of MC moves
class Ising():
	
	def __init__(self,N,amp):
		self.N=N
		self.lattice = 2*np.random.randint(1,size=(N,N))-1 #Spins
		self.lattice.resize(N,N)
		self.amp=amp	#Problem Instance values
		self.E0=0		#Initial Energy
		self.E=0		#Instantaneous Energy (During Iterations)
		self.sums=0		#Sum of spins*amplitudes

	def initEnergy(self):
		'''Calculates Initial Energy'''
		e=np.sum(self.lattice*self.amp)
		self.sums=e
		self.E0=e*e
		print("Initial energy=",self.E0)
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
		for i in range(nmoves+1):
			self.mcmove(1.0/temp)
			
		                    

l=np.random.randint(100,size=(N,N)) #Problem Instance

rm=Ising(N,l)
print("Initial config:")
print(rm.amp*rm.lattice)
rm.initEnergy()
rm.simulate(temp,nmoves)
print("Final energy=",rm.E)
print("Final config:")
print(rm.amp*rm.lattice)
