CC=g++
DEBUG =
OPT = -O2

INCLUDE = ./ntl-6.0.0/include/
LIBS = -lm ./ntl-6.0.0/lib/libntl.a 

WARN = -Wno-deprecated

OBJ = main.o braid.o

htop : $(OBJ)
	$(CC) $(WARN) $(OPT) $(DEBUG) $(OBJ) $(LIBS) -o $@

main.o : main.cpp
	$(CC) $(WARN) $(OPT) $(DEBUG) -c -I $(INCLUDE) main.cpp -o $@

braid.o : braid.h braid.cpp 
	$(CC) $(WARN) $(OPT) $(DEBUG) -c -I $(INCLUDE) braid.cpp -o $@

clean :
	rm -f htop* *.o *~


