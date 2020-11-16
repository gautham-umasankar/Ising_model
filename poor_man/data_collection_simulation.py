import numpy as np
import matplotlib.pyplot as plt
import sys
import os 
import csv
from datetime import datetime

N = 3           # Number of spins
N_iters = 30   # Number of iterations per run
N_runs = 10     # Number of runs 

max_alpha = 2.0
min_alpha = 0.5
alpha_step = 0.1
plot_alpha = 1.5

max_beta = 2.0
min_beta = 0.1
beta_step = 0.1
plot_beta = (max_beta + min_beta)/2

sig = 0.04          # Std Deviation of noise
offset = 0.5    
V_pi_DC = 1.0       # V_pi_DC of the modulator
V_pi_RF = 0.5       # V_pi_RF of the modulator
I0 = 1.0            # Action of photodiode and MZM
DC_bias = V_pi_DC/4    # Action of DC_bias

scaling = 1.0

J_file = 0          #Flags which enable and disable different features
bifurcation = 0 
trajectory = 0
legend = 0
solver = 0
data_file = 0
tanh_switch = 1

sig_low = 0.001
sig_high = 0.1
sig_step = 0.005

instance_directory = 0

field_names = ["Instance_name","Nodes","Edges","Density","Lowest_Weight","Largest_Weight","Mean_Weight","alpha","beta","scaling","offset","noise_sigma","run_number","iteration_number","cut_value","optimal_cut","fraction_reached","Timestamp","Datestamp"] 

sim_vs_data = 1
#print("sys.argv is :",sys.argv)
if(len(sys.argv)>1):
    i = 1
    while(i <= len(sys.argv[1:])):
        opt = str(sys.argv[i])
        #print(opt)

        #Alpha parameters
        if(opt == '-aplt'):
            plot_alpha = float(sys.argv[i+1])
        elif(opt == '-ua'):
            max_alpha = float(sys.argv[i+1])
        elif(opt == '-da'):
            min_alpha = float(sys.argv[i+1])
        elif(opt == '-sa'):
            alpha_step = float(sys.argv[i+1])
        elif(opt == '-a'):
            min_alpha = float(sys.argv[i+1])
            max_alpha = min_alpha + 1e-10
            plot_alpha = min_alpha
        
        #Beta parameters
        elif(opt == '-bplt'):
            plot_beta = float(sys.argv[i+1])
        elif(opt == '-ub'):
            max_beta = float(sys.argv[i+1])
        elif(opt == '-db'):
            min_beta = float(sys.argv[i+1])
        elif(opt == '-sb'):
            beta_step = float(sys.argv[i+1])
        elif(opt == '-b'):
            min_beta = float(sys.argv[i+1])
            max_beta = min_beta + 1e-10
            plot_beta = min_beta

       #Run parameters
        elif(opt == '-iter'):
            N_iters = int(sys.argv[i+1])
        elif(opt == '-N'):
            N = int(sys.argv[i+1])
        elif(opt == '-run'):
            N_runs = int(sys.argv[i+1])

        #Modulator parameters
        elif(opt == '-dc'):
            DC_bias = float(sys.argv[i+1])
        elif(opt == '-o'):
            offset = float(sys.argv[i+1])
        elif(opt == '-I'):
            I0 = float(sys.argv[i+1])
            scaling = 1/I0
        elif(opt == '-pidc'):
            V_pi_DC = float(sys.argv[i+1])
        elif(opt == '-pirf'):
            V_pi_RF = float(sys.argv[i+1])

        #Scaling parameters
        elif(opt == "-scaling"):
            scaling = float(sys.argv[i+1])
        
        #Noise parameters
        elif(opt == '-sig'):
            sig = float(sys.argv[i+1])
        elif(opt == '-sig_low'):
            sig_low = float(sys.argv[i+1])
        elif(opt == '-sig_high'):
            sig_high = float(sys.argv[i+1])
        elif(opt == '-sig_step'):
            sig_step = float(sys.argv[i+1])

        #Config parameters        
        elif(opt == '-J'):
            J_file = str(sys.argv[i+1])
        elif(opt == '-sol'):
            solver = 1
        elif(opt == '-traj'):
            trajectory = 1
        elif(opt == '-legend'):
            legend = 1
        elif(opt == '-bif'):
            bifurcation = 1
        elif(opt == '-notanh'):
            tanh_switch = 0

        #Instance Directory
        elif(opt == '-instance_directory'):
            instance_directory = str(sys.argv[i+1])
        elif(opt == '-data'):
            data_file = str(sys.argv[i+1])
            sim_vs_data = 0
        
        i+=1

def modulator(x):
    return I0*pow(np.cos(np.pi*x/V_pi_RF + DC_bias*np.pi/(V_pi_DC)),2) #Check this equation later

def feedback(x,alpha,beta,J):
    J = beta*J
    np.fill_diagonal(J,-alpha)
    return (J)@x

def notanh(x):
    for i,val in enumerate(x):
        if(val>=1):
            x[i] = 1
        elif(val<=-1):
            x[i] = -1
    return x        
    #return np.array([1 if i>=1 else -1 if i<=-1 else i for i in x])

def cut_value(cut,J):
    value = 0
    for row,i in enumerate(cut):
        for col,j in enumerate(cut):
            if(i*j<0):
                #print(i,j)
                #print("Row and column",row,col,J[row][col])
                value = value + J[row][col]
    return value/2

def load_J(J_file):
    #Function to read and return J
    print("The file's path is: ",J_file)
    f = open(J_file,"r")
    N, number_of_edges = [int(i) for i in f.readline().split()]
    J = np.zeros([N,N])
    lines = f.readlines()
    w_list = []
    number_of_edges = len(lines)
    for line in lines:
        l = line.split()
        r,c,w = int(l[0])-1, int(l[1])-1, float(l[2])
        J[r][c] = w
        J[c][r] = w
        w_list.append(w)        
    f.close()
    w = np.array(w_list)
    return N,number_of_edges,(2*number_of_edges/(N*(N-1))),J,np.amin(w),np.amax(w),sum(w)/number_of_edges

#Create Alpha and Beta arrays
Alpha = np.arange(min_alpha,max_alpha,alpha_step)
Beta = np.arange(min_beta,max_beta,beta_step)

if(data_file):
    if not os.path.isfile(data_file):
        print("The file doesn't already exist")
        with open(data_file,'w') as file_main:
            writer = csv.DictWriter(file_main,fieldnames = field_names) 
            writer.writeheader()
    with open(data_file,'a') as file_main:   
        print(data_file)
        print(type(data_file))     
        writer = csv.DictWriter(file_main,fieldnames = field_names) 
        list_instances = os.listdir(instance_directory)
        list_instances.sort()
        for instance in list_instances:
            N, number_of_edges, density, J, min_weight, max_weight, mean_weight = load_J(os.path.join(os.getcwd(),instance_directory,instance))
            for alpha in Alpha:
                for beta in Beta:
                    for noise_sig in np.arange(sig_low,sig_high,sig_step):
                        for run in range(N_runs):
                            x_k = np.zeros([N,1])
                            x_in = np.zeros([N,1])
                            print("Working with instance = {}, Alpha = {}, Beta = {}, Noise_Sig = {}, Run = {}, ".format(instance,alpha,beta,noise_sig,run))
                            i = 0
                            traj_x = x_in
                            noise = np.random.normal(0,noise_sig,[N,N_iters])
                            while(i < N_iters):
                                # This is the calculated value to be put out to the DAC
                                
                                x_out = 2*np.around((feedback(x_k, alpha,beta,J) + np.array([noise[:,i]]).T)/2,3)
                                #x_out = x_out*V_pi_RF/np.pi
                                
                                if(tanh_switch):
                                    x_in = 2*np.around(modulator(np.tanh(x_out))/2,3)
                                else:
                                    x_in = 2*np.around(modulator(notanh(x_out))/2,3)

                                # The state value
                                x_k = scaling*x_in-offset

                                i += 1
                                
                                #Write to file
                                write_dict = {}
                                write_dict["Instance_name"] = instance
                                write_dict["Nodes"] = N
                                write_dict["Edges"] = number_of_edges
                                write_dict["Density"] = density
                                write_dict["Lowest_Weight"] = min_weight
                                write_dict["Largest_Weight"] = max_weight
                                write_dict["Mean_Weight"] = mean_weight
                                write_dict["alpha"] = alpha
                                write_dict["beta"] = beta
                                write_dict["scaling"] = scaling
                                write_dict["offset"] = offset
                                write_dict["noise_sigma"] = noise_sig
                                write_dict["run_number"] = run
                                write_dict["iteration_number"] = i
                                write_dict["cut_value"] = cut_value(x_k[:,-1],J)
                                write_dict["Timestamp"] = str(datetime.now().time())
                                write_dict["Datestamp"] = str(datetime.now().date())
                                if instance.split('_')[0] == 's':
                                    #print("Square lattice")
                                    write_dict["optimal_cut"] = 2*np.sqrt(float(instance.split('_')[1].split('.')[0]))*(np.sqrt(float(instance.split('_')[1].split('.')[0]))-1)
                                    write_dict["fraction_reached"] = write_dict["cut_value"]/write_dict["optimal_cut"]
                                writer.writerow(write_dict)

else:
        
    #Initialize J to a ring if there is no input
    J = np.zeros([N,N])
    for i in range(1,N-1):
        J[i][i+1] = 1
        J[i][i-1] = 1

    if(N >= 2):
        J[0][1] = 1
        J[0][N-1] = 1
        J[N-1][N-2] = 1
        J[N-1][0] = 1

    #Load J from a file
    if(J_file):
        N,number_of_edges,density,J,dummy1,dummy2,dummy3 = load_J(J_file)

    switch = 0
    final_x = np.zeros([N,1])
    pre_final_x = np.zeros([N,1])
    traj_x = np.zeros([N,1])

    solutions = []
    for alpha in Alpha:
        for beta in Beta:
            for run in range(N_runs):
                x_k = np.zeros([N,1])
                x_in = np.zeros([N,1])
                #print("Working with Alpha = {}, Beta = {}, Run = {}".format(alpha,beta,run))
                i = 0
                traj_x = x_in
                noise = np.random.normal(0,sig,[N,N_iters])
                while(i < N_iters):
                    # This is the calculated value to be put out to the DAC
                    # Put the coupled equation instead later
                    x_out = 2*np.around((feedback(x_k, alpha,beta,J) + np.array([noise[:,i]]).T)/2,3)
                    #x_out = x_out*V_pi_RF/np.pi
                    #print((x_out.T).shape)
                    #print(type(x_out))
                    #print("notanh(x_out)",notanh(x_out))
                    # The value received from the modulator
                    if(tanh_switch):
                        x_in = 2*np.around(modulator(np.tanh(x_out))/2,3)
                    else:
                        x_in = 2*np.around(modulator(notanh(x_out))/2,3)

                    # print(x_in)
                    # The state value
                    x_k = scaling*x_in-offset

                    # print("x_k = ",x_k)
                    i += 1
                    # Add to trajectory
                    traj_x = np.c_[traj_x,x_k]
                
                if(abs(np.around(alpha,3) - plot_alpha)<alpha_step and switch == 0 and abs(np.around(beta,3) - plot_beta)<beta_step):
                    # plot trajectory   
                    #print("Plot alpha = {}\nAlpha = {}\nPlot Beta = {}\nBeta = {}\n".format(plot_alpha,alpha,plot_beta,beta))
                    switch = 1
                    if(trajectory):
                        plot1 = plt.figure(1)
                        for i in range(N):
                            plt.plot(traj_x[i],"-",label = "spin {}".format(i))
                        if(legend):
                            plt.legend()       
                        plt.title("Run number = {} Alpha = {} Beta = {} Cut = {}".format(run,np.around(alpha,3),np.around(beta,3),cut_value(np.sign(traj_x[:,-1],J))))
                        plt.xlabel("Iteration Number")
                        plt.ylabel("State of the spin")
            
                pre_final_x = np.c_[pre_final_x,traj_x[:,-2]]
                final_x = np.c_[final_x,traj_x[:,-1]]
                cut = np.sign(traj_x[:,-1])
                if(solver):
                    solutions.append([alpha,beta,run,cut_value(cut,J),cut,traj_x])

    if(bifurcation):
        plot2 = plt.figure(2)
        index_alpha = np.where(abs(Beta-plot_beta)<beta_step)[0][0]
        plt.plot(Alpha,final_x[:,1::N_runs*len(Beta)].T,"r.",markersize=1.5)#[:np.where(plot_beta<Beta)[0][0]]
        plt.title("Final value vs Alpha at beta = 0")
        plt.xlabel("Alpha")
        plt.ylabel("Final Value")
        # Include plot against beta here. Need to choose an alpha for this
        plot3 = plt.figure(3)
        try:
            index_alpha = np.where(abs(Alpha-plot_alpha)<alpha_step)[0][0]
        except:
            plt.show()
            exit(0)
        index_alpha *= N_runs*len(Beta)
        plt.plot(Beta,final_x[:, index_alpha:index_alpha+N_runs*len(Beta):N_runs].T,"r.",markersize=1.5)
        plt.title("Final value vs Beta at alpha = {}".format(plot_alpha))
        plt.xlabel("Beta")
        plt.ylabel("Final Value")
        # # plt.plot(Alpha,pre_final_x[:,1:].T,"r.",markersize=0.5)

    if(solver):
        opt_cut_value = -1e10
        opt_alpha = 0
        opt_beta = 0
        opt_cut = np.zeros(N)
        for i in solutions:
            if (i[3] > opt_cut_value):
                opt_cut_value = i[3]
                opt_alpha = i[0]
                opt_beta = i[1]
                opt_cut = i[4]
                opt_traj = i[5]
        # print("The nodes on two sides of the cut are: ")
        # print("The negative side:")
        # for node,i in enumerate(opt_cut):
        #     if(i<0):
        #         print(node+1)
        # print("The positive side:")
        # for node,i in enumerate(opt_cut):
        #     if(i>0):
        #         print(node+1)
        # #print(cut)
        # print("The number of positive nodes is: ",len(np.where(opt_cut>0)[0]))
        # print("The number of negative nodes is: ",len(np.where(opt_cut<0)[0]))

        print("The best cut value that was obtained is: ")
        print("Cut value: {}\nAlpha: {}\nBeta: {}".format(opt_cut_value,opt_alpha,opt_beta))
        plot4 = plt.figure(4)
        for i in range(N):
            plt.plot(opt_traj[i],"-",label = "spin {}".format(i))
        if(legend):
            plt.legend()       
        plt.title("Best solution. Alpha = {} Beta = {} Cut = {}".format(np.around(opt_alpha,3),np.around(opt_beta,3),opt_cut_value))
        plt.xlabel("Iteration Number")
        plt.ylabel("State of the spin")


    plt.show()