#! /usr/bin/env python
import os
import math  
from scipy import stats

# MAIN PARAMETERS
nrun = 100
duration = 175
anticipation = 20

tracefile = "sim.res"


righe = ["" for n in range(100)]
riga = 0
righe[riga] =  "# example input file of corona"; riga = riga+1
righe[riga] =  "1979    Random number generator seed"; rigaseed = riga; riga = riga+1
righe[riga] =  "1       run simulation (0/1)"; riga = riga+1
righe[riga] =  "sim     name of simulation"; riga = riga+1
righe[riga] =  "9       Debug level"; riga = riga+1
righe[riga] =  "0       Transient duration (s) (0 = none, -1 = cache full)"; riga = riga+1
righe[riga] =  "0       Batch duration (s) (0 = no batch analysis; 1 = equal to transient)"; riga = riga+1
righe[riga] =  "175     Simulation duration (s) (zero = determined by batch analysis)"; rigaduration = riga; riga = riga+1
righe[riga] =  "0.01    Sample interval (s) (zero = none)"; riga = riga+1
righe[riga] =  "1.0	Print  interval (s) (zero = none)"; riga = riga+1
righe[riga] =  "1	type of incub (unif 0, tri 1)"; riga = riga+1
righe[riga] =  "2	minincubtime"; riga = riga+1
righe[riga] =  "12	maxincubtime"; riga = riga+1
righe[riga] =  "6	aveincubtime (if applicable)"; riga = riga+1
righe[riga] =  "2	presicktime"; riga = riga+1
righe[riga] =  "0	type of sick time (unif 0, tri 1)"; riga = riga+1
righe[riga] =  "2	minsicktime"; riga = riga+1
righe[riga] =  "12	maxsicktime"; riga = riga+1
righe[riga] =  "7	avesicktime (if applicable)"; riga = riga+1
righe[riga] =  "0	type of death time (unif 0, tri 1)"; riga = riga+1
righe[riga] =  "2	mindeathtime"; riga = riga+1
righe[riga] =  "12	maxdeathtime"; riga = riga+1
righe[riga] =  "7	avedeathtime (if applicable)"; riga = riga+1
righe[riga] =  "0       type of residual (exp 0, lognormal 1)"; riga = riga+1
righe[riga] =  "10.0	averesidual"; riga = riga+1
righe[riga] =  "0.0	sigmaresidual (if applicable)"; riga = riga+1
righe[riga] =  "0.05	low infection rate"; riga = riga+1
righe[riga] =  "1.0	high infection rate"; riga = riga+1
righe[riga] =  "0.91	asymptomatic probability"; riga = riga+1
righe[riga] =  "0.03	super-diffusor probability"; riga = riga+1
righe[riga] =  "0.4	intensive therapy and death prob"; riga = riga+1
righe[riga] =  "0.2	intensive therapy and recover prob"; riga = riga+1
righe[riga] =  "370	initial number of positives"; riga = riga+1
righe[riga] =  "2	type of factor"; riga = riga+1
righe[riga] =  "3.71	factor param1"; riga = riga+1
righe[riga] =  "0.315   factor param2"; riga = riga+1
righe[riga] =  "30	factor param3"; riga = riga+1
righe[riga] =  "-3	factor param4"; riga = riga+1
righe[riga] =  "70      factor param5"; riga = riga+1
righe[riga] =  "100	factor param6"; riga = riga+1
righe[riga] =  "1.0	factor param7"; riga = riga+1
righe[riga] =  "2	type of swabs"; riga = riga+1
righe[riga] =  "0	swabs param1"; riga = riga+1
righe[riga] =  "0.000115 swabs param2"; riga = riga+1
righe[riga] =  "60	swabs param3"; riga = riga+1
righe[riga] =  "0	computeR0 (0/1)"; riga = riga+1
righe[riga] =  "1.0	binR0"; riga = riga+1
righe[riga] =  "-1	guard time to compute R0 (-1 = maxI + maxD)"; riga = riga+1
righe[riga] =  "20.0	anticipation time"; rigaanticipation = riga; riga = riga+1

print("Number of lines in input file: ",riga)   

sum_tot_ = list() 
sum_delta_ = list() 
sum_totk_ = list() 
sum_deltak_ = list() 

sum2_tot_ = list() 
sum2_delta_ = list() 
sum2_totk_ = list() 
sum2_deltak_ = list() 

for t in range(duration):
	sum_tot_.append(0)
	sum2_tot_.append(0)
	sum_delta_.append(0)
	sum2_delta_.append(0)
	sum_totk_.append(0)
	sum2_totk_.append(0)
	sum_deltak_.append(0)
	sum2_deltak_.append(0)

righe[rigaduration] =  str(duration) + "\tSimulation duration (s) (zero = determined by batch analysis)"
righe[rigaanticipation] =  str(anticipation) + "\tanticipation time"


run = 1
while run <= nrun:
	

	seed = 1974 + run
	righe[rigaseed] = str(seed) + "\tRandom number generator seed"       
            
	f = open("inputmulti","w")
	for n in range(riga):
		print(righe[n],file=f)
	f.close()

	print("Running simulation ",run," with seed = ",seed,"...")
	os.system('./corona < inputmulti > /dev/null')
	print("done!...")     
    
	fi = open(tracefile,"r")    
	row = fi.readline()
	row = fi.readline();
	row = fi.readline();
	
	while row != "":
	
		# print("letta riga: ",row)
		pieces = row.split("\t");
		day = int(float(pieces[0]))
		day = day + anticipation
		
			
		num_tot_ = int(pieces[1]);
		num_delta_ = int(pieces[2]);
		num_totk_ = int(pieces[3]);
		num_deltak_ = int(pieces[4]);
		
		sum_tot_[day] = sum_tot_[day] + num_tot_; 
		sum_delta_[day] = sum_delta_[day] + num_delta_;
		sum_totk_[day] = sum_totk_[day] + num_totk_;
		sum_deltak_[day] = sum_deltak_[day] + num_deltak_;

		sum2_tot_[day] = sum2_tot_[day] + num_tot_*num_tot_; 
		sum2_delta_[day] = sum2_delta_[day] + num_delta_*num_delta_;
		sum2_totk_[day] = sum2_totk_[day] + num_totk_*num_totk_;
		sum2_deltak_[day] = sum2_deltak_[day] + num_deltak_*num_deltak_;
			
		if day == 174:
			break	
		row = fi.readline();
	
	fi.close()
	run = run + 1

        
fo = open("multi.res","w")

conf = 0.95;
factor = stats.t.interval(conf,nrun-1,0,1)[1]*1.0/math.sqrt(nrun)
print("factor = ",factor);

print("# results for tot",file=fo)
for t in range(duration):
	mean = sum_tot_[t]*1.0/nrun;
	sigma = math.sqrt((sum2_tot_[t] - nrun*mean*mean)*1.0/(nrun-1));
	inf = mean-sigma*factor
	sup = mean+sigma*factor
	print(t-anticipation,mean,sigma,sigma*factor,inf,sup,sep='  \t',file=fo)
print("\n\n",file=fo)

print("# results for delta",file=fo)
for t in range(duration):
	mean = sum_delta_[t]*1.0/nrun;
	sigma = math.sqrt((sum2_delta_[t] - nrun*mean*mean)*1.0/(nrun-1));
	inf = mean-sigma*factor
	sup = mean+sigma*factor
	print(t-anticipation,mean,sigma,sigma*factor,inf,sup,sep='  \t',file=fo)
print("\n\n",file=fo)

print("# results for totk",file=fo)
for t in range(duration):
	mean = sum_totk_[t]*1.0/nrun;
	sigma = math.sqrt((sum2_totk_[t] - nrun*mean*mean)*1.0/(nrun-1));
	inf = mean-sigma*factor
	sup = mean+sigma*factor
	print(t-anticipation,mean,sigma,sigma*factor,inf,sup,sep='  \t',file=fo)
print("\n\n",file=fo)

print("# results for deltak",file=fo)
for t in range(duration):
	mean = sum_deltak_[t]*1.0/nrun;
	sigma = math.sqrt((sum2_deltak_[t] - nrun*mean*mean)*1.0/(nrun-1));
	inf = mean-sigma*factor
	sup = mean+sigma*factor
	print(t-anticipation,mean,sigma,sigma*factor,inf,sup,sep='  \t',file=fo)
print("\n\n",file=fo)

    
fo.close()
    
    
    
