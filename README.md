# Ising Machines
This project is about Ising Machines, which are programs/devices which settle naturally to the ground state of a given Ising Hamiltonian:
<p align = "center">
<img src= https://latex.codecogs.com/gif.latex?H%20%3D%20-%5Csum_%7Bi%3D1%7D%5E%7BN%7Dh_%7Bi%7D%5Csigma_%7Bi%7D%20-%20%5Csum_%7Bi%2Cj%3D1%7D%5E%7Bi%2Cj%3DN%7DJ_%7Bi%2Cj%7D%5Csigma_%7Bi%7D%5Csigma_%7Bj%7D />
</p>

Many NP-Complete problems like [Number Partioning](https://en.wikipedia.org/wiki/Partition_problem) and the [Max-cut](https://en.wikipedia.org/wiki/Maximum_cut) problem can be reframed into an Ising Hamiltonian which we can optimize. We attempt to create different Ising Machines and benchmark them versus classical heuristic methods like Metropolis Monte-Carlo (MC) Sampling Methods. At present, we are close to creating a Coherent Ising Machine. We have also worked on MC Sampling (Python Code for the Number Partioning (NP-complete) and the Max-cut (NP-Complete) Problems) and an initial attempt at using GPUs for the same. 
  
The details of the Ising Machines/ simulations/ heuristics that have been implemented so far is as follows:

## Coherent Ising Machines
The first physical Ising Machine we are implementing is a Coherent Ising Machine. We implement a setup along the lines of the work done in [this paper by Bohm. et al.](https://www.nature.com/articles/s41467-019-11484-3). We use an Opto-Electronic Oscillator (OEO), whose state naturally settles at or very near the optimum solution to the problem. Our Repository contains Python code for our simulation of the experiment. It also contains C-code which was used to program the FPGA used in our experimental setup.

## Heuristics based on the Monte Carlo Metropolis Method
Python code was written to simulate the thermodynamics of an Ising Hamiltonian. With different techniques like simulated annealing, we can make the system settle very close to the optimum. More details are within the folders. 
