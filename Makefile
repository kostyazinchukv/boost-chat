CXX = g++

CXXFLAGS += -g -std=c++11 -Wall -pthread 

all: server

clean: 
	rm -fr *.o *.a

re: clean all

server: ./include/server.hpp
	$(CXX) $(CXXFLAGS) -lboost_filesystem ./src/server.cpp ./src/mainServer.cpp