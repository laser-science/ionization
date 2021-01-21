# ionization
strong field ionization of atoms and molecules

ToftoEnergy is the c++ code used once the time of flight bin data from the experiment has been converted into kinetic energy bins.
Once this is done, this code is used to assign the hits into evenly spaced bins in energy.
The code can only run either the 'fast' peak or the 'slow' peak at one time, and which peak a particular .dat file represents must be known before running.
the code will prompt the user to enter the file name, designate between fast or slow peak, and state the size of the bins desired, in whole eV.
