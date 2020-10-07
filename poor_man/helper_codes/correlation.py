import numpy as np 
import matplotlib.pyplot as plt

import sys

shifts = np.array(range(-60,60,1))
if(len(sys.argv)>1):
    i = 1
    while(i <= len(sys.argv[1:])):
        opt = str(sys.argv[i])
        #print(opt)

        #Alpha parameters
        if(opt == '-data'):
            data_file = str(sys.argv[i+1])
        elif(opt == '-shift'):
            shift = float(sys.argv[i+1])
        '''elif(opt == '-da'):
            min_alpha = float(sys.argv[i+1])
        elif(opt == '-sa'):
            alpha_step = float(sys.argv[i+1])
        elif(opt == '-a'):
            min_alpha = float(sys.argv[i+1])
            max_alpha = min_alpha + 1e-10
            plot_alpha = min_alpha'''
        i+=1

f = open(data_file)

lines = f.readlines()[2:]
f.close()
buff_size = 16384
iters = int(len(lines)/buff_size)

x_in = np.array([float(i.split()[3]) for i in lines])
x_out = np.array([float(i.split()[2]) for i in lines])
correlations = []
for i in range(iters):
    fig = plt.figure(i)
    corr = np.array([sum(x_in[i*buff_size:(i+1)*buff_size]*np.roll(x_out[i*buff_size:(i+1)*buff_size],shift)) for shift in range(-60,60,1)])/(sum(x_out[i*buff_size:(i+1)*buff_size]))
    correlations.append(np.argmax(corr))
    plt.title("Iteration: {}".format(i+1))
    plt.plot(shifts,corr)

plt.show()

for i,corr in enumerate(correlations):
    print("Iteration {} shift = {}".format(i,corr))