# http_server makefile

COMP = g++
FLAGS = -std=c++17 -pthread

all: server.o client.o request.o response.o
	$(COMP) $(FLAGS) main.cpp -o server $^

server.o: server.h server.cpp
request.o: request.h request.cpp
response.o: response.h response.cpp
client.o: client.h client.cpp

clean:
	rm -f server *.o
