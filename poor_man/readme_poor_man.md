# Coherent Ising Machines
Coherent Ising Machines are based on Opto-Electronic Oscillators (OEO). They are generally created with feedback loops consisting of non-linear optical elements and electronic devices which sample optical output and generate electrical inputs. 
We have implemented an experiment along the lines of [this work](https://www.nature.com/articles/s41467-019-11484-3) by Bohm et al. As shown in the figure below, the electronics captures
the photovoltage, processes it and then generates a feedback voltage to the modulator. This repository contains C-code for programming a [RedPitaya](https://www.redpitaya.com/) to perform these functions.

This repository also contains a Python based simulation of the same experiment.

# Simulation Usage
The simulation code is called `simulation.py`. It is a `Python3` program which can be run with the following command line arguments
1. -N Number of spins 
2. -iter Number of iterations
3. -run Number of runs per value of alpha and beta
4. -a single value of alpha
5. -b single value of beta
6. -ua, -da, -sa Upper, lower limits and step size in a sweep of alpha (Replace 'a' by 'b', -ua becomes -ub and so on for a sweep of alpha)
7. -J File name for input of adjacency matrix in the GSet format
8. -traj Option for enabling the plotting of a trajectory
9. -sol Option for finding the best cut in the given range of alpha and beta
10. -bif Option for enabling the plotting of a bifurcation
11. -sig Option for setting the standard deviation of the Gaussian noise added to the feedback signals

For other options, please refer the code.

Example of use:
```python3 simulation.py -N 100 -iter 30 -run 2 -sig 0.04 -ua 2.0 -da 0.1 -sa 0.1 -ub 1.0 -db 0.1 -sb 0.1 -J Adjacency.txt -traj -sol -bif 
```
This runs the code for an Adjacency matrix (In file Adjacency.txt) with 100 nodes, 30 iterations, 2 runs per value of alpha and beta. Alpha is swept from 0.1 to 2.0 in steps of 0.1. Beta is swept from 0.1 to 1.0 in steps of 0.1. The trajectory of the spins,the best solution obtained and its trajectory and a bifurcation plot (Final value vs Alpha, Final value vs Beta) are plotted. The 

