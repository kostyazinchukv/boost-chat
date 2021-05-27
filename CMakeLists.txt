cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "std=c++${CMAKE_CXX_STANDARD} -pthread")
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "-pthread")

project(CHAT CXX)

set(SERVER_PROJECT_FILES

    ./include/server.hpp
    
    ./src/server.cpp
    ./src/mainServer.cpp

)

set(CLIENT_PROJECT_FILES

    ./include/client.hpp
    
    ./src/client.cpp
    ./src/mainClient.cpp

)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(SERVER
    ${SERVER_PROJECT_FILES}
)
add_executable(CLIENT
    ${CLIENT_PROJECT_FILES}
)
