import numpy as np
import matplotlib.pyplot as plt
import sys


N = 2
N_alpha = 200
N_iters = 100
max_alpha = 4
plot_alpha = 2
offset = 0.5
sig = 0.04
V_pi = 1 # change this later
beta = 1.0

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
        elif(opt == 'b'):
            beta = float(sys.argv[i+1])
        elif(opt == 'N'):
            N = int(sys.argv[i+1])
        sys.argv.pop(i+1)
        i+=1

J = beta*np.ones([N,N])
np.fill_diagonal(J,0)

def modulator(x):
    return pow(np.cos(x/V_pi + 0.25*np.pi),2)

def feedback(x,alpha):
    np.fill_diagonal(J,-alpha)
    return J@x

Alpha = np.linspace(-max_alpha,max_alpha,N_alpha)
ii = np.where(plot_alpha<Alpha)[0][0]
final_x = np.zeros([N,1])
pre_final_x = np.zeros([N,1])
traj_x = np.zeros([N,1])

for alpha in Alpha:
    x_k = np.zeros([N,1])
    x_in = np.zeros([N,1])
    print("Working with Alpha = ", alpha,"...")
    i = 0
    traj_x = x_in
    noise = np.random.normal(0,sig,[N,N_iters])
    while(i < N_iters):
        # This is the calculated value to be put out to the DAC
        # Put the coupled equation instead later
        x_out = 2*np.around((feedback(x_k, alpha) + np.array([noise[:,i]]).T)/2,3)

        # The value received from the modulator
        x_in = 2*np.around(modulator(np.tanh(x_out))/2,3)

        # The state value
        x_k = x_in-offset

        # print("x_k = ",x_k)
        i += 1
        # Add to trajectory
        traj_x = np.c_[traj_x,x_k]
    
    if(np.where(alpha==Alpha)==ii):
        # plot trajector
        plot1 = plt.figure(1)
        plt.plot(traj_x[0],"b-")
        plt.plot(traj_x[1],"r-")

    # print("x_in = ",x_in)
    # print("__________")
    pre_final_x = np.c_[pre_final_x,traj_x[:,-2]]
    final_x = np.c_[final_x,traj_x[:,-1]]

plot2 = plt.figure(2)
plt.plot(Alpha,final_x[:,1:].T,"r.",markersize=1)
# # plt.plot(Alpha,pre_final_x[:,1:].T,"r.",markersize=0.5)
plt.show()
