import numpy as np
import matplotlib.pyplot as plt
import sys


N = 3
alpha_step = 0.1
N_iters = 100
max_alpha = 4
min_alpha = -4
plot_alpha = 2
offset = 0.5
sig = 0.04
V_pi = 1 # change this later
I0 = 1   # Represents action of photodiode and MZM. Change this later
beta = 1.0
J_file = 0
bifurcation = 0 #Flags which enable and disable relevant plots
trajectory = 0
solver = 0
phase_shift = 0.25


if(len(sys.argv)>1):
    i = 1
    while(i < len(sys.argv[1:])):
        opt = str(sys.argv[i])
        #print(opt)
        if(opt == '-t'):
            plot_alpha = float(sys.argv[i+1])
        elif(opt == '-u'):
            max_alpha = float(sys.argv[i+1])
        elif(opt == '-d'):
            min_alpha = float(sys.argv[i+1])
        elif(opt == '-a'):
            min_alpha = float(sys.argv[i+1])
            max_alpha = min_alpha + 1e-10
            plot_alpha = min_alpha
        elif(opt == '-p'):
            alpha_step = float(sys.argv[i+1])
        elif(opt == '-ps'):
            phase_shift = float(sys.argv[i+1])
        elif(opt == '-iter'):
            N_iters = int(sys.argv[i+1])
        elif(opt == '-o'):
            offset = float(sys.argv[i+1])
        elif(opt == '-s'):
            sig = float(sys.argv[i+1])
        elif(opt == '-b'):
            beta = float(sys.argv[i+1])
        elif(opt == '-N'):
            N = int(sys.argv[i+1])
        elif(opt == '-J'):
            J_file = str(sys.argv[i+1])
        elif(opt == '-sol'):
            solver = 1
            min_alpha = float(sys.argv[i+1])
            max_alpha = min_alpha + 1e-10
            plot_alpha = min_alpha
            #print("Plot alpha for solver is: ",plot_alpha)
        elif(opt == '-traj'):
            trajectory = int(sys.argv[i+1])
        elif(opt == '-bif'):
            bifurcation = int(sys.argv[i+1])
        elif(opt == '-pa'):
            plot_alpha = float(sys.argv[i+1])

        sys.argv.pop(i+1)
        i+=1

J = beta*np.ones([N,N])
np.fill_diagonal(J,0)
if(J_file):
    f = open(J_file,"r")
    J = np.zeros([N,N])
    for i,line in enumerate(f.readlines()):
        J[i,:] = np.array([float(i) for i in line.split()])
    J = beta*J
    #print(J)
    f.close()


def modulator(x):
    return I0*pow(np.cos(x/V_pi + phase_shift*np.pi),2)

def feedback(x,alpha):
    np.fill_diagonal(J,-alpha)
    return J@x

Alpha = np.arange(min_alpha,max_alpha,alpha_step)
#ii = np.where(plot_alpha<Alpha)[0][0]
switch = 0
final_x = np.zeros([N,1])
pre_final_x = np.zeros([N,1])
traj_x = np.zeros([N,1])

for alpha in Alpha:
    x_k = np.zeros([N,1])
    x_in = np.zeros([N,1])
    #print("Working with Alpha = ", alpha,"...")
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
    
    if(trajectory):
        if(plot_alpha<=alpha and switch == 0):
            # plot trajectory
            switch = 1
            plot1 = plt.figure(1)
            for i in range(N):
                plt.plot(traj_x[i],"-",label = "spin {}".format(i))
            #plt.legend()       
            plt.title("Alpha = {}".format(alpha))
    # print("x_in = ",x_in)
    # print("__________")
    pre_final_x = np.c_[pre_final_x,traj_x[:,-2]]
    final_x = np.c_[final_x,traj_x[:,-1]]

#print(traj_x)


if(bifurcation):
    plot2 = plt.figure(2)
    plt.plot(Alpha,final_x[:,1:].T,"r.",markersize=1)
    # # plt.plot(Alpha,pre_final_x[:,1:].T,"r.",markersize=0.5)

if(solver):
    #print(traj_x[:,-2],traj_x[:,-1])
    print("The solution is as follows: \n")
    cut = np.sign(final_x[:,-1])
    cut_value = 0
    for row,i in enumerate(cut):
        for col,j in enumerate(cut):
            if(i*j<0):
                cut_value = cut_value + J[row][col]/beta

    print("The nodes on two sides of the cut are: ")
    print("The negative side:")
    for node,i in enumerate(cut):
        if(i<0):
            print(node+1)
    print("The positive side:")
    for node,i in enumerate(cut):
        if(i>0):
            print(node+1)
    print("The cut value is: ",cut_value/2)
    
plt.show()
