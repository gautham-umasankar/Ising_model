#number partitioning in ising model

import numpy as np
from numpy.random import rand
import matplotlib.pyplot as plt
import time


plt.ion()
f = plt.figure(figsize=(18, 10)) # plot the calculated values

class Ising():
	
	def __init__(self,N,amp):
		self.N=N
		self.lattice = np.ones(N*N)
		self.lattice.resize(N,N)
		self.amp=amp
		self.E0=0
		self.E=0
		self.sums=0

	def initEnergy(self):
		e=np.sum(self.lattice*self.amp)
		self.sums=e
		self.E0=e*e
		print("Initial energy=",self.E0)
		self.E=e*e

	def calcenergy(self,a,b):
		"""print("Prev energy=",self.E)
		print("Previous sum=",self.sums)
		print("s=",self.lattice[a][b])
		print("n=",self.amp[a][b])"""

		e=-4*self.amp[a][b]*self.lattice[a][b]*(self.sums-(self.amp[a][b]*self.lattice[a][b]))
		return e
    
	def mcmove(self,beta):
		for i in range(N+1):
			for j in range(N+1):
				a = np.random.randint(0, self.N)
				b = np.random.randint(0, self.N)
				s =  self.lattice[a, b]
				cost=self.calcenergy(a,b)
				#print("Cost of flip:",cost)
				if cost < 0:	
					s *= -1
					self.sums=self.sums-2*self.amp[a][b]*self.lattice[a][b]
					self.E+=cost
					#print("Flip taken.")
				elif rand() < np.exp(-cost*beta):
					s *= -1
					self.sums=self.sums-2*self.amp[a][b]*self.lattice[a][b]
					self.E+=cost
					#print("Flip taken")"""

				"""print("New energy=",self.E)
				print("New lattice:")
				print(self.amp*self.lattice)"""
				self.lattice[a, b] = s

	def simulate(self):   
		temp     = 30
			
		msrmnt = 50
		for i in range(msrmnt+1):
			self.mcmove(1.0/temp)
			self.configPlot(i)
		                    
	def configPlot(self,i):
		X, Y = np.meshgrid(range(self.N+1), range(self.N,-1,-1))
		plt.pcolormesh(X, Y, self.lattice*self.amp, cmap=plt.cm.RdBu);
		plt.title('Time=%d    Energy=%f'%(i,self.E));plt.axis('tight')    
		plt.show()
		plt.pause(0.5)

N=3
#l=2*np.random.randint(1, size=(N,N))-1
l=np.random.randint(100,size=(N,N))
#l=np.ones(N*N)
l.resize(N,N)

rm=Ising(N,l)
print("Initial config:")
print(rm.amp*rm.lattice)
rm.initEnergy()
rm.simulate()
print("Final energy=",rm.E)
print("Final config:")
print(rm.amp*rm.lattice)
plt.ioff()
plt.show()


