import numpy as np
import matplotlib.pyplot as plt
import sys

N = 100
N_alpha = 100
N_iters = 48
max_alpha = 4
plot_alpha = 0.5

def modulator(x):
    # return pow(np.cos(x + 0.25*np.pi),2)
    return pow(np.cos(x + 0.25*np.pi),2)-0.5

if(len(sys.argv)==2):
    plot_alpha = float(sys.argv[1])

Alpha = np.linspace(0,max_alpha,N_alpha)
ii = np.where(plot_alpha<Alpha)[0][0]
beta = 0.0
final_x = np.zeros([N,1])
traj_x = np.zeros([N,1])

for alpha in Alpha:
    x_k = np.ones([N,1])
    x_n = np.zeros([N,1])
    old_x_k = x_k
    print("Working with Alpha = ", alpha,"...")
    i = 0
    traj_x =x_n
    while(i < N_iters):
        x_k = 2*np.around(x_n/2,3)
        # noise = np.random.normal(0,0.0004,[N,1])
        noise = np.random.normal(0,0.04,[N,1])
        # put the coupled equation instead
        x_n = 2*np.around(modulator(alpha*x_k + noise)/2,3)
        # print("x_n = ",x_n)
        i+=1
        traj_x = np.c_[traj_x,x_n]
    
    if(np.where(alpha==Alpha)==ii):
        plot1 = plt.figure(1)
        plt.plot(traj_x[1],"rx-")#,markersize=0.5)

    print("x_n = ",x_n)
    print("__________")
    final_x = np.c_[final_x,x_n]

plot2 = plt.figure(2)
plt.plot(Alpha,final_x[:,1:].T,"r.",markersize=0.5)
plt.show()
