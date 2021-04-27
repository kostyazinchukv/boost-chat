#include "../include/server.hpp"
#include <vector>
#include <stdlib.h>
#include <fstream>

Server::Server()
{
    host = "127.0.0.1";
    port = 8888;
}
Server::~Server()
{
    ioc.stop();
}

void Server::menu()
{
    std::string command;
    boost::asio::ip::tcp::socket socket(ioc);
    for(;;)
    {
        std::cout<<"Client$ "<<std::flush;
        std::getline(std::cin, command);
        if(command == "help")
        {
            help();
        }
        if(command == "start")
        {
            start(socket);
        }
        if(command == "stop")
        {
            stop(socket);
        }
        if(command == "exit")
        {
            exitSession(socket);
        }

        if((command != "help") && (command != "connect") && (command != "send") && (command != "exit"))
        {

        }
    }
}
void Server::start(boost::asio::ip::tcp::socket& sock)
{    
    std::cout<<"Server$ "<<std::flush;
    boost::system::error_code ec;
    std::vector<char> buff;
    buff.resize(1024);
    boost::asio::ip::tcp::endpoint address({}, port);
    boost::asio::ip::tcp::acceptor acc(ioc, address);
    boost::asio::ip::tcp::socket socket(ioc);
    
    acc.accept(socket);
    for(;;)
    {
        if(ec)
        {
            std::cerr<<"accept "<<ec<<std::endl;
            return;
        }

        auto t = socket.read_some(boost::asio::buffer(buff), ec);
        if(ec)
        {
            std::cout<<"Read "<<t<<" "<<ec.message()<<std::endl;
            return;
        }
        for(int i = 0; i<t; i++)
        {
            std::cout<<buff[i]<<std::flush;
        }

    }
}
void Server::stop(boost::asio::ip::tcp::socket& sock)
{
    sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}
void Server::exitSession(boost::asio::ip::tcp::socket& sock)
{
    sock.close();
    exit(0);
}
void Server::help()
{
    std::string info = "Usage: <command>\nCommands:";
    info += "\n\thelp --> This message\n";
    info += "\n\tstart --> Open socket\n";
    info += "\n\tstop <message>--> stop recieving/sending on socket\n";
    info += "\n\texit --> close server session\n";
    info += "\n\tsetport <int> --> set value to port parametr";
    std::cout<<info<<std::flush;
}
void Server::setPort(int p)
{
    port = p;
    std::ofstream portFile;
    portFile.open("port.txt");
    if(!portFile.is_open())
    {
        std::cout<<"Cannot open the file to write port value"<<std::endl;
        return;
    }
    else
    {
        portFile << std::to_string(port);
    }
    portFile.close();

}


