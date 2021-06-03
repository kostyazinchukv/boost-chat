cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "-pthread")
set(LINK_TYPE_STATIC ON)


project(CHAT CXX)

set(SERVER_PROJECT_FILES
    ./src/mainServer.cpp
)

set(CLIENT_PROJECT_FILES
    ./src/mainClient.cpp
)

if(${LINK_TYPE_STATIC})
    add_library(server_lib STATIC
        ./include/server.hpp
        ./src/server.cpp
    )

    add_library(client_lib STATIC
        ./include/client.hpp
        ./src/client.cpp
    )
else()
    add_library(server_lib SHARED
        ./include/server.hpp
        ./src/server.cpp
    )
    add_library(client_lib SHARED
        ./include/client.hpp
        ./src/client.cpp
    )
endif()


find_package(Boost REQUIRED COMPONENTS system)

add_executable(SERVER
    ${SERVER_PROJECT_FILES}
)

add_executable(CLIENT
    ${CLIENT_PROJECT_FILES}  
)

target_include_directories(client_lib PUBLIC ./include)
target_include_directories(server_lib PUBLIC ./include)

target_link_libraries(SERVER PRIVATE server_lib)
target_link_libraries(CLIENT PRIVATE client_lib)