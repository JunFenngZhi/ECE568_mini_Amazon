CC=g++
CFLAGS=-g
EXTRAFLAGS=-lpqxx -lpq -lpthread


all: server

server: Server.h exception.h socket.h 
	$(CC) $(CFLAGS)  -o server main.cpp Server.cpp $(EXTRAFLAGS)

.PHONY:
	clean
clean:
	rm -rf *.o server  