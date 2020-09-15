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
<img width = 600 height = 450 src = "https://github.com/gautham-umasankar/Ising_model/blob/master/poor_man/plots/simulation/bifurcation_100_spins_v2.png">
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

The simulation reached a cut-value of .... which is within ...% of the optimum.


We are at a stage of the experiment where the CIM is working for networks of 4 spins, but not for larger networks. We are fixing the problem currently. One of the key challenges of the experiment is to synchronize the generation and acquisition of voltage-pulses. Shown below is a plot of generated voltage (mention colour) and acquired voltage when we do a loopback emulation (We just connect the output and the input ports of the FPGA and simulate the intensity modulator's action in our C-code). The delay parameters in the C-code were adjusted to make sure that the waveforms match properly.
<!---![alt text](https://github.com/gautham-umasankar/Ising_model/poor_man/plots/....png)--->
