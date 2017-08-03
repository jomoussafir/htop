CC=g++
DEBUG =
OPT = -O2

LIBS = -lm -lntl

WARN = -Wno-deprecated

OBJ = main.o braid.o

htop : $(OBJ)
	$(CC) $(WARN) $(OPT) $(DEBUG) $(OBJ) $(LIBS) -o $@

main.o : main.cpp
	$(CC) $(WARN) $(OPT) $(DEBUG) -c main.cpp -o $@

braid.o : braid.h braid.cpp 
	$(CC) $(WARN) $(OPT) $(DEBUG) -c braid.cpp -o $@

clean :
	rm -f htop* *.o *~


