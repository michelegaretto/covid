
CC = g++
CCFLAGS = -Wall -O3 -Warray-bounds=0 -Wno-unused-variable
LIBS = -L/home/pi/gsl/lib -lgsl -lgslcblas -lm
OBJECTS = main.o service.o scheduler.o timer-handler.o simulation.o virus.o 

corona : $(OBJECTS)
	$(CC) $(CCFLAGS) -o corona $(OBJECTS) $(LIBS)
main.o : main.cc 
	$(CC) $(CCFLAGS) -c main.cc
service.o :  service.cc 
	$(CC) $(CCFLAGS) -c service.cc		
scheduler.o : scheduler.cc
	$(CC) $(CCFLAGS) -c scheduler.cc
timer-handler.o : timer-handler.cc
	$(CC) $(CCFLAGS) -c timer-handler.cc				
simulation.o : simulation.cc
	$(CC) $(CCFLAGS) -c simulation.cc
virus.o : virus.cc
	$(CC) $(CCFLAGS) -c virus.cc	
	
clean	:
	  rm -f $(OBJECTS)
	 
