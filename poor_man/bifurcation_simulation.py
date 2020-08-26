import numpy as np
import matplotlib.pyplot as plt
import sys

N = 100
N_alpha = 100
N_iters = 48
max_alpha = 4
plot_alpha = 1.1
offset = 0.5
sig = 0.04

def modulator(x):
    return pow(np.cos(x + 0.25*np.pi),2)

if(len(sys.argv)>1):
    i = 1
    while(i < len(sys.argv[1:])):
        opt = str(sys.argv[i])
        print(opt)
        if(opt == 't'):
            plot_alpha = float(sys.argv[i+1])
        elif(opt == 'm'):
            max_alpha = float(sys.argv[i+1])
        elif(opt == 'o'):
            offset = float(sys.argv[i+1])
        elif(opt == 's'):
            sig = float(sys.argv[i+1])
        sys.argv.pop(i+1)
        i+=1

Alpha = np.linspace(0,max_alpha,N_alpha)
ii = np.where(plot_alpha<Alpha)[0][0]
beta = 0.0
final_x = np.zeros([N,1])
pre_final_x = np.zeros([N,1])
traj_x = np.zeros([N,1])
noise = np.random.normal(0,sig,[N,N])

for alpha in Alpha:
    x_k = np.zeros([N,1])
    x_n = np.zeros([N,1])
    print("Working with Alpha = ", alpha,"...")
    i = 0
    traj_x =x_n
    noise = np.random.normal(0,sig,[N,N_iters])
    while(i < N_iters):
        # This is the calculated value to be put out to the DAC
        # Put the coupled equation instead later
        x_out = 2*np.around((alpha*x_k + np.array([noise[:,i]]).T)/2,3)

        # The value received from the modulator
        x_n = 2*np.around(modulator(np.tanh(x_out))/2,3)

        # The state value
        x_k = x_n-offset

        # print("x_k = ",x_k)
        i+=1
        # Add to trajectory
        traj_x = np.c_[traj_x,x_k]
    
    if(np.where(alpha==Alpha)==ii):
        # plot trajector
        plot1 = plt.figure(1)
        plt.plot(traj_x[1],"rx-")#,markersize=0.5)

    print("x_n = ",x_n)
    print("__________")
    pre_final_x = np.c_[pre_final_x,traj_x[:,-2]]
    final_x = np.c_[final_x,traj_x[:,-1]]

plot2 = plt.figure(2)
plt.plot(Alpha,final_x[:,1:].T,"r.",markersize=0.5)
# plt.plot(Alpha,pre_final_x[:,1:].T,"r.",markersize=0.5)
plt.show()
