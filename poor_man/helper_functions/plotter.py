import termplotlib as tpl
import sys
import numpy as np

argc = len(sys.argv)

if(argc == 1):
    print("Enter input file and type of plot")

elif(argc == 2):
    # print("")
    plt_type = sys.argv[1]
    if(plt_type == 'l'):
        f = open("./../out_data/x_n.txt","r")
        data = f.readlines()
        data = list(np.float_(data))
        iteration = list(range(len(data)))

        if(iteration == 0):
            exit(0)
        fig = tpl.figure()
        fig.plot(iteration, data, label="data", width=50, height=15)
        fig.show()
        f.close()

    elif(plt_type == 'h'):
        f = open("./../out_data/bifurcation.csv","r")
        data = f.readlines()
        data = list(np.float_(data))
        print(data)
        if(len(data) == 0):
            exit(0)
        counts, bin_edges = np.histogram(data)
        fig = tpl.figure()
        fig.hist(counts, bin_edges, orientation="horizontal", force_ascii=False)
        fig.show()
    
    else:
        print("Enter type of plot")

elif(argc == 3):
    # print("")
    plt_type = sys.argv[1]
    if(plt_type == 'l'):
        f = open(sys.argv[2],"r")
        data = f.readlines()
        data = list(np.float_(data))
        iteration = list(range(len(data)))
        if(iteration == 0):
            exit(0)

        fig = tpl.figure()
        fig.plot(iteration, data, label="data", width=50, height=15)
        fig.show()
        f.close()

    elif(plt_type == 'h'):
        f = open(sys.argv[2],"r")
        data = f.readlines()
        data = list(np.float_(data))

        if(len(data) == 0):
            exit(0)
        counts, bin_edges = np.histogram(data)
        fig = tpl.figure()
        fig.hist(counts, bin_edges, orientation="horizontal", force_ascii=False)
        fig.show()

        fig = tpl.figure()
        fig.hist(counts, bin_edges, grid=[15, 25], force_ascii=False)
        fig.show()
    
    else:
        print("Enter type of plot")

else:
    print("Too many arguments")