#
# This project contains the event-driven simulator and data files used to derive results of paper:
# "A time-modulated Hawkes process to model the spread of COVID-19 and the impact of countermeasures"
# by Michele Garetto (michele.garetto@unito.it), Emilio Leonardi (emilio.leonardi@polito.it), Giovanni-Luca Torrisi (giovanniluca.torrisi@cnr.it)
# 
# The simulator is written in C++, and requirs the GSL open-source library
# There is a minimal Makefile, that can be used to compile the simulator by issuing the command:
# ./make 
# which produces the executable "corona"
# Parameters are then conveniently passed through by an input file, such as:
# ./corona < input
#


# The sub-directory "first" contains additional files related to the first wave of COVID in Italy:
# A python script multi.py is used to produce multiple runs and collect results on file multi.res
# which is later plotted using gnuplot

# The sub-directory "second" contains additional files related to the second wave of COVID in Italy:
# A different virus.cc file to obtain reversely fitted profiles mu(t) and rho(t)
# Mobility data: 

# All code is made available under GNU GPL v3 license.
# For inquires please contact Michele Garetto, michele.garetto@unito.it
  

