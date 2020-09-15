from __future__ import division
import numpy as np
from numpy.random import rand
import pylab as pl
# nset=np.array([ 14, 238,  30,  12, 452, 106,  31, 189, 205,  21, 416, 321, 169,
#        185,  31, 269, 480, 406, 164, 404, 263, 469, 317, 144, 299,  90,
#        361,  55, 179, 120,  61,  98, 277, 311, 343, 432, 496,  81, 108,
#        125,   4, 191, 127, 311, 256,  21, 339, 253, 152,  19, 295, 241,
#        364, 401,  68,  83,  10, 465, 205, 241, 192, 336, 333, 355, 158,
#        144, 130, 455, 440,  56, 429, 300, 407, 352, 309, 129, 170, 475,
#        374, 411, 394,  32, 358, 302, 324,  83, 171, 491,  56, 281, 374,
#        208, 176,  67, 402, 420, 296, 381, 189, 256, 100, 149, 288, 369,
#        301,  96,  25, 490, 138, 355, 180, 244, 291, 194, 335, 454, 220,
#        261, 107, 150,   2, 120, 493, 453,  25, 168,  86,  18, 467, 278,
#        373,  71, 369, 188, 445, 305, 342, 131,  59, 337, 253,  94,  35,
#        366, 320,  82, 228, 240, 179, 404, 381, 207, 456, 322, 248, 485,
#        360, 399, 309,  22, 261, 383, 145,   7, 215,  65, 160, 111,   8,
#        305, 162, 412,  94, 175, 301, 144, 345,   2, 271, 326, 111,  32,
#        269, 217, 353,  12, 133, 288, 362, 257,   7, 200, 118, 325, 178,
#        197, 198, 489, 109, 362]) #problem instance
T=0.25 #temperature
nsize=50 #Max Size of the problem
# nstep=20000 #Number of steps
def initialstate(N):   
    ''' generates a random spin configuration for initial condition'''
    state = 2*np.random.randint(2, size=N)-1
    return state

def mcmove(config, beta):
    '''Monte Carlo move using Metropolis algorithm '''
    a = np.random.randint(0, N)
    s =  config[a]
    cost = 4*(np.sum(nset*config)-s*nset[a])*s*nset[a]
    if cost > 0:
        s *= -1
    elif rand() < np.exp(cost*beta):
        s *= -1
    config[a] = s
    return config

def calcEnergy(config,nset):
    '''Energy of a given configuration'''
    energy = np.sum(nset*nset)
    for i in range(len(config)):
    	for j in range(i+1,len(config)):
    		energy+=2*config[i]*config[j]*nset[i]*nset[j]	
        
    return float(energy)

def calcMag(config):
    '''Magnetization of a given configuration'''
    mag = np.sum(config)
    return mag

Efinal=np.zeros(nsize)
iterations=np.zeros(nsize)
instances=np.array(range(50,nsize+50))
for instance in instances:
	nset=np.random.randint(instance*2,size=instance)
	N=len(nset)
	config=initialstate(N)
	for j in range(10):
		Ei=calcEnergy(config,nset)
		Ef=0.0
		count=0
		while(abs(Ei-Ef)>min(nset)):
			count=count+1
			Ei=Ef
			for i in range(1,len(nset)):
				mcmove(config,1/T)
				Ef=(Ef*(1-1/i)+calcEnergy(config,nset)/i)
			if count>10000:
				break	
		iterations[instance-51]+=count*(instance-1)
		Efinal[instance-51]+=calcEnergy(config,nset)		
	iterations[instance-51]/=10
	Efinal[instance-51]/=10		

pl.plot(instances,Efinal,'bo')
pl.title("Final Energy vs Size of Problem (Linear Scale)")
pl.xlabel("Size of Problem")
pl.ylabel("Number of Iterations")
pl.grid(True)
pl.show()

pl.plot(instances,iterations,'ro')
pl.title("Number of Iterations vs Size of Problem (Linear Scale)")
pl.xlabel("Size of Problem")
pl.ylabel("Number of Iterations")
pl.grid(True)
pl.show()

pl.semilogx(instances,iterations,'ro')
pl.title("Number of Iterations vs Log of Size of Problem")
pl.xlabel("Size of Problem")
pl.ylabel("Number of Iterations")
pl.grid(True)
pl.show()

pl.loglog(instances,iterations,'ro')
pl.title("Number of Iterations vs Size of Problem (Loglog Scale)")
pl.xlabel("Size of Problem")
pl.ylabel("Number of Iterations")
pl.grid(True)
pl.show()

#print("The final configurations is: \n",config)
#print("\n",nset,"\nThese are the numbers")
# print("\nThe final energy is: ",calcEnergy(config))
# print("\nThe number of iterations taken is: ",count*len(nset)-count)
# print("\nThe final magnetization is: ",calcMag(config))