import numpy as np 
import matplotlib.pyplot as plt

import sys

class correlation_calc:
    def __init__(self, file):
        self.f = open(data_file)
    
    def correlate(self, buff_in, buff_out, shifts):
        return np.array([sum(buff_in*np.roll(buff_out,shift)) for shift in shifts])/(np.sqrt(sum(buff_out*buff_out))*np.sqrt(sum(buff_in*buff_in)))
    
    def calc(self, iteration = -1, shifts = np.array(range(-60,60,1))):
        lines = self.f.readlines()[2:]
        self.f.close()
        buff_size = 16384
        window_size = 2048
        iters = int(len(lines)/buff_size)

        x_in = np.array([float(i.split()[3]) for i in lines])
        x_out = np.array([float(i.split()[2]) for i in lines])
        correlations = []
        for i in range(iters):
            # fig = plt.figure(i)
            buff_out = x_out[i*buff_size:(i+1)*buff_size]
            buff_in = x_in[i*buff_size:(i+1)*buff_size]
            corr = self.correlate(buff_in, buff_out, shifts)
            correlations.append(shifts[np.argmax(corr)])
            # plt.title("Iteration: {}".format(i+1))
            # plt.plot(shifts,corr)
            if(i == iteration):
                corrs = []
                fig = plt.figure()
                # plt.plot(buff_out)
                # plt.plot(buff_in)
                # plt.show()
                for j in range(int(buff_size/window_size)):
                    # fig = plt.figure(j)
                    shifts_2 = np.array(range(-20,20,1))
                    t_out = buff_out[j*window_size:(j+1)*window_size]
                    t_in = buff_in[j*window_size:(j+1)*window_size]
                    c = self.correlate(t_in, t_out, shifts_2)
                    corrs.append(shifts_2[np.argmax(c)])
                    # plt.title("Window: {}".format(j+1))
                    plt.plot(shifts_2,c, label = "Window:{}".format(j))
                
                for i,corr in enumerate(corrs):
                    print("Window {} shift = {}".format(i,corr))
                plt.legend()
                plt.show()

        # plt.show()

        for i,corr in enumerate(correlations):
            print("Iteration {} shift = {}".format(i,corr))

if __name__ == "__main__":
    shifts = np.array(range(-60,60,1))
    iteration = -1
    if(len(sys.argv)>1):
        i = 1
        while(i <= len(sys.argv[1:])):
            opt = str(sys.argv[i])
            #print(opt)

            #Alpha parameters
            if(opt == '-data'):
                data_file = str(sys.argv[i+1])
            elif(opt == '-shift'):
                shifts = float(sys.argv[i+1])
            elif(opt == "-iteration"):
                iteration = int(sys.argv[i+1])
            '''elif(opt == '-da'):
                min_alpha = float(sys.argv[i+1])
            elif(opt == '-sa'):
                alpha_step = float(sys.argv[i+1])
            elif(opt == '-a'):
                min_alpha = float(sys.argv[i+1])
                max_alpha = min_alpha + 1e-10
                plot_alpha = min_alpha'''
            i+=1
    c = correlation_calc(data_file)
    c.calc(iteration, shifts)