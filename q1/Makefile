EXEC=a.out
CC=mpicc
CXX=mpic++ 
DBG_CFLAGS= -D_DEBUG -g 
OBJ=source.o
CFLAGS= -std=c++98 -Wall -fno-builtin -O1 $(DBG_CFLAGS) 
CXXFLAGS= $(CFLAGS)

all: $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $(EXEC)

%.o: %.c %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $< 

.PHONY: clean

clean:
	rm -f *.o $(EXEC)
