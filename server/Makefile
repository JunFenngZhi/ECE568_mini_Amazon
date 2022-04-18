GDB = -g
THREAD = -pthread
PQXX = -lpqxx -lpq
GPB = `pkg-config --cflags --libs protobuf`
PROTO_CC = ./protobuf/world_amazon.pb.cc
PROTO_H = ./protobuf/world_amazon.pb.h
VERSION = -std=c++11

all: server

server: Server.h exception.h Order.h socket.h AResponseHandler.h OrderProcess.h sql_function.h Warehouse.h protobufCommunication.hpp ThreadSafe_queue.h $(PROTO_H)
	g++ -o server main.cpp Server.cpp sql_function.cpp AResponseHandler.cpp OrderProcess.cpp socket.cpp $(PROTO_CC) $(PQXX) $(GDB) $(THREAD) $(GPB) $(VERSION)

.PHONY:
	clean
clean:
	rm -rf *.o server  