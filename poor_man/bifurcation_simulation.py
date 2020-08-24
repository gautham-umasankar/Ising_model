import numpy as np
import matplotlib.pyplot as plt

N = 10
N_alpha = 1000

def modulator(x):
    return pow(np.cos(x - 0.25*np.pi),2) - 1/2


Alpha = np.linspace(0,3,N_alpha)
beta = 0.0
final_x = np.zeros([N,1])

for alpha in Alpha:
    x_k = np.ones([N,1])
    x_n = np.zeros([N,1])
    old_x_k = x_k
    print("Working with Alpha = ", alpha,"...")
    i = 0
    while(i < 100):
        x_k = 2*np.around(x_n/2,3)
        noise = np.random.normal(0,0.004,[N,1])
        # put the coupled equation instead
        x_n = 2*np.around(modulator(alpha*x_k + noise)/2,3)
        # print("x_n = ",x_n)
        i+=1

    print("x_n = ",x_n)
    print("__________")
    final_x = np.c_[final_x,x_n]

plt.plot(Alpha,final_x[:,1:].T,"r.",markersize=0.5)
plt.show()
