COMP = g++
FLAGS = -std=c++11 -pthread 

VPATH = src/

all: main.o sentif.o server_base.o web_server.o admin.o sockio.o
	$(COMP) $(FLAGS) $^ -o sentif

main.o: main.cpp
sentif.o: sentif.h sentif.cpp
server_base.o: server_base.h server_base.cpp
web_server.o: web_server.h web_server.cpp
admin.o: admin.h admin.cpp
sockio.o: sockio.h sockio.cpp

clean:
	rm -f *.o sentif

