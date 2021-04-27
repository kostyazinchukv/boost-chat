CXX = g++

CXXFLAGS += -g -std=c++11 -pthread 


all: server client

clean: 
	rm -fr *.o *.a

re: clean all


server: ./include/server.hpp
	$(CXX) $(CXXFLAGS) -lboost_filesystem ./src/server.cpp ./src/mainServer.cpp -o srv

client: ./include/client.hpp
	$(CXX) $(CXXFLAGS) -lboost_filesystem ./src/client.cpp ./src/mainClient.cpp -o cl
