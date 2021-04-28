#include "../include/server.hpp"
#include "../include/client.hpp"

int main(int argc, char* argv[])
{
    Server server;
    server.setPort(1111);
    server.menu();
    return 0;
}