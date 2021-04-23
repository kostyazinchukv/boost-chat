#include "../include/server.hpp"
#include "../include/client.hpp"

int main(int argc, char* argv[])
{
    std::cout<<"init\n";
    Server server;
    std::cout<<"portSet\n";
    server.setPort(9999);
    std::cout<<"start\n";
    server.start();
    return 0;
}