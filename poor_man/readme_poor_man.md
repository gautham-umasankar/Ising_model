# Coherent Ising Machines
Coherent Ising Machines are based on Opto-Electronic Oscillators (OEO). They are generally created with feedback loops consisting of non-linear optical elements and electronic devices which sample optical output and generate electrical inputs. 
We have implemented an experiment along the lines of [this work](https://www.nature.com/articles/s41467-019-11484-3) by Bohm et al. As shown in the figure below, the electronics captures
the photovoltage, processes it and then generates a feedback voltage to the modulator. This repository contains C-code for programming a [RedPitaya](https://www.redpitaya.com/) to perform these functions.

This repository also contains a Python based simulation of the same experiment.

When the non-linear dynamics indicated by the equations below are followed, the system naturally exhibits a pitch-fork bifurcation in the photo-voltage as the feedback factor alpha increases, which is shown in the plots below. 

This is a bifurcation as seen in simulation:
<p align = "center">
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/simulation/bifurcation_100_spins_v2.png">
</p>
<!---![Bifurcation as seen in simulation](https://github.com/gautham-umasankar/Ising_model/tree/master/poor_man/plots/simulation/bifurcation_100_spins_v2.png)--->

This is a bifurcation as seen in the experiment:
<!---![alt text](https://github.com/gautham-umasankar/Ising_model/poor_man/plots/....png)--->
<p align = "center">
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/experiment/bifurcation_plot_uncoupled.png">
</p>

Once the matrix J is initialized to the relevant adjacency matrix, the system naturally solves the maxcut problem, since the maxcut problem can be recast into the Ising Hamiltonian with the coupling strenghts in the Ising Hamiltonian the same as the adjacency matrix. Show below are some relevant plots:

This is the trajectory of 2 spins in the simulation, when their coupling is set such that they anti-align. Note that beta is set to be negative, instead of setting the entries in the adjacency matrix to negative values.
<!---![alt text](https://github.com/gautham-umasankar/Ising_model/poor_man/plots/....png)--->
<p align = "center">
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/simulation/2_spin_anti_align_trajectory.png">
</p>

This is the trajectory of 16 spins in the experiment, with the coupling set to a randomly generated max-cut problem of 16 nodes:
<!---![alt text](https://github.com/gautham-umasankar/Ising_model/poor_man/plots/....png)--->
<p align = "center">
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/experiment/16_spins/trajectory_alpha_1_5_beta_0_5.png">
</p>

This is the trajectory of 16 spins in the simulation, with the same coupling as above, when they reach the optimal solution of the above adjacency matrix:
<!---![alt text](https://github.com/gautham-umasankar/Ising_model/poor_man/plots/....png)--->

<p align = "center">
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/simulation/16_spin_opt_cut_trajectory.png">
</p>

This is the trajectory of 100 spins in the simulation, with the coupling set to a randomly generated max-cut problem of 100 nodes:
<!---![alt text](https://github.com/gautham-umasankar/Ising_model/poor_man/plots/....png)--->

<p align = "center">
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/simulation/100_spin_opt_cut_trajectory.png">
</p>
The simulation reached a cut-value of 2008 which is 99.46% of the optimum value of 2019, which was calculated using exact solvers. 


We are at a stage of the experiment where the CIM is working for networks of 4 spins, but not for larger networks, as can be seen above in a plot of the trajectory of 16 spins, which oscillate instead of bifurcate. We are fixing the problem currently. 

# Loop Back Emulation
One of the key challenges of the experiment is to synchronize the generation and acquisition of voltage-pulses. Shown below is a plot of generated voltage (mention colour) and acquired voltage when we do a loopback emulation (We just connect the output and the input ports of the FPGA and simulate the intensity modulator's action in our C-code). The delay parameters in the C-code were adjusted to make sure that the waveforms match properly.
<!---![alt text](https://github.com/gautham-umasankar/Ising_model/poor_man/plots/....png)--->

<p align = "center">
<img width = 900 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/loopback/synchronisation_30_spins_zoomed_in.png">
</p>

One interesting effect in the loop-back setup is that there is an attenuation caused by connections from the output to the input port. Fortunately, this is linear and can be corrected for. The following is a plot of x_in vs x_out. We can clearly see the linear nature of the line. When we fit a straight line through the data, was found the slope was 0.88. We use this value in the code to correct for this.

<p align = "center">
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/loopback/loopback_attenuation_m_0_884979_c_0_0541717.png">
</p>

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

For other options, please refer the code.



