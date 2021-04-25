COMP = g++
FLAGS = -std=c++11

vpath %.h   include
vpath %.cpp src

all: sentif.h sentif.cpp
	$(COMP) $(FLAGS) $^ -o sentif

clean:
	rm -f *.o sentif
