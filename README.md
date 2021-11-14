# Ising Machines
This project is about Ising Machines, which are programs/devices which settle naturally to the ground state of a given Ising Hamiltonian:
<p align = "center">
<img src= https://latex.codecogs.com/gif.latex?H%20%3D%20-%5Csum_%7Bi%3D1%7D%5E%7BN%7Dh_%7Bi%7D%5Csigma_%7Bi%7D%20-%20%5Csum_%7Bi%2Cj%3D1%7D%5E%7Bi%2Cj%3DN%7DJ_%7Bi%2Cj%7D%5Csigma_%7Bi%7D%5Csigma_%7Bj%7D />
</p>

Many NP-Complete problems like [Number Partioning](https://en.wikipedia.org/wiki/Partition_problem) and the [Max-cut](https://en.wikipedia.org/wiki/Maximum_cut) problem can be reframed into an Ising Hamiltonian which we can optimize. We attempt to create different Ising Machines and benchmark them versus classical heuristic methods like Metropolis Monte-Carlo (MC) Sampling Methods, "gurobi" (a commercial optimisation sotware), as well the Dwave quantum annealer. These results are covered in our IEEE article: [Benchmarking the Poor Man's Ising Machine](https://ieeexplore.ieee.org/document/9505840).
  
The details of the Ising Machines/ simulations/ heuristics that have been implemented so far is as follows:

## Heuristics based on the Monte Carlo Metropolis-Hastings Method
Python code was written to simulate the thermodynamics of an Ising Hamiltonian. With different techniques like simulated annealing, we can make the system settle very close to the optimum. We use the Monte Carlo Metropolis method to solve for above two problems. These codes are in the [Monte Carlo](https://github.com/gautham-umasankar/Ising_model/tree/newrp/Monte_Carlo) folder. For more information about the Monte Carloe Metropolis-Hastings algorithm, refer to [this Wiki page](https://en.wikipedia.org/wiki/Metropolis%E2%80%93Hastings_algorithm). These simulations were done to understand the Ising model for optimisation problems better, as well as understanding these two specific NP problems in more depth. It is a good starting point for anyone interested in learning to solve optimisation problems using the Ising formulation. 

## Coherent Ising Machines
The physical Ising Machine we are implementing is a Coherent Ising Machine(CIM). We implement a setup along the lines of the work done in [this paper by Bohm. et al.](https://www.nature.com/articles/s41467-019-11484-3). We use an Opto-Electronic Oscillator (OEO), whose state naturally settles at or very near the optimum solution to the problem. A schematic of our setup is shown below: <p align = "center">
<img src= https://github.com/gautham-umasankar/Ising_model/blob/newrp/poor_man/Images/Experimental_setup_schematic.jpg>
</p>
While the actual setup, with the latest components, is shown below:
<p align = "center">
<img src= https://github.com/gautham-umasankar/Ising_model/blob/newrp/poor_man/Images/HD-Schematic-1.jpg>
</p>

This repository also contains the code for our implementation of this version of the CIM, most of which can be found in which can be found in the 
[poor_man](https://github.com/gautham-umasankar/Ising_model/tree/newrp/poor_man) folder. We first implemented a python simulation of the dynamics of the system: [simulation.py](https://github.com/gautham-umasankar/Ising_model/blob/newrp/poor_man/simulation.py). The usage details of this can be found in the [README](https://github.com/gautham-umasankar/Ising_model/blob/newrp/poor_man/README.md) inside the folder itself. Some of the simulation results can be found in our paper. 

After having done the simulations of our system we proceeded to code for its actual physical realisation. This was done in 2 stages, with us working on a simple loopback before proceeding on to incorporating the lasers in the setup. The ADC, DAC and feedback in our system is done through a [Red Pitaya](https://redpitaya.com/). We first started off with a STEMlab 125-10, and then moved on to the STEMlab 250-14 afterwards(shown in image above). 

The file [poor_man.c](https://github.com/gautham-umasankar/Ising_model/blob/newrp/poor_man/poor_man.c) is the core code of our implementation. It contains the code for both loopback and the actual run on the experimental setup with the lasers, although there are a few lines to change as the case may be(specifically line 318 needs to be commented, and the inequalities need to be reversed in the function "find_shift" for loopback). The usage details are again in [README](https://github.com/gautham-umasankar/Ising_model/blob/newrp/poor_man/README.md). 

While our first implemenation used the CPU on the Red Pitaya for the feedback, it was evident that the matrix-vector product required for the feedback was a bottleneck for our speeds. This initial version used the C drivers that were available through the Red Pitaya Open source [apis](https://github.com/RedPitaya/RedPitaya/tree/e49883b3d49788c88f92b0649f1c194f5b2f412f/api). Once this version was working as expected, we switched over to using a combination of the FPGA and CPU that was available on the board. We offloaded the matrix-vector product onto the FPGA to utilise the parallelism, and continued to use the convenience of the apis to communicate with the ADC and DAC peripherals. We used an AXI interface and appropriate memory mapping to communicate with the accelerator that we had design on the FPGA.

The FPGA design was done using Vivado HLS and then generating the bitstream in Vivado. Our base project was the one provided by Red Pitaya for the respective boards, which included all the functionality for the apis(this is the v0.94 project). We added our IP block generated through Vivado HLS for the matrix-vector product accelerator and connected it to the ZYNQ processor in the block diagram of the project through an AXI interconnect. Suitable memory mapping then allowed us to access this part of the FPGA from C code on the red pitaya, through the driver functions that Vivado HLS generates (after of course loading the appropriate bitstream). The source code for the HLS part can be found in [poor_man_fpga](https://github.com/gautham-umasankar/Ising_model/tree/newrp/poor_man_FPGA). Since we used a fixed point representation to improve our speed, we developed a new version of our core code: [poor_man_fp.c](https://github.com/gautham-umasankar/Ising_model/blob/newrp/poor_man_FPGA/poor_man_fp.c). Please note that appropriate changes to the files must be made corresponding to the exact HLS design used. The HLS design is configurable with the amount of parallelism implemented , but this must be done during syntehsis itself. On the other hand, the number of spins can be configured during run time. 

## Acknowledgements

We would like to thank Prof. Sridhar Tayur from Carnegie Mellon University for fruitful discussions on the project. We would also like to thank Red Pitaya for the support provided in the Free Student Red Pitaya tryout for our initial implementations. 

