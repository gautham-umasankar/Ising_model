# Coherent Ising Machines
Coherent Ising Machines are based on Opto-Electronic Oscillators (OEO). They are generally created with feedback loops consisting of non-linear optical elements and electronic devices which sample optical output and generate electrical inputs. 
We have implemented an experiment along the lines of [this work](https://www.nature.com/articles/s41467-019-11484-3) by Bohm et al. As shown in the figure below, the electronics captures
the photovoltage, processes it and then generates a feedback voltage to the modulator. This repository contains C-code for programming a [RedPitya](https://www.redpitaya.com/) to perform these functions.

This repository also contains a Python based simulation of the same experiment.

When the non-linear dynamics indicated by the equations below are followed, the system naturally exhibits a pitch-fork bifurcation in the photo-voltage, which is shown below. 

