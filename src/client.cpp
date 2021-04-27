#include "../include/client.hpp"
#include <stdlib.h>
Client::Client(int p, std::string h, std::string d) 
: port(p), host(h), data(d)
{
    message.resize(1000);
}

Client::~Client()
{
    ioc.stop();
}

void Client::createMessage(std::vector<char>& container, std::string data)
{
    std::string header = "Content-Lenght: " + std::to_string(data.size());
    for(int i = 0; i<header.size(); i++)
    {
        container.push_back(header[i]);
    }
    container.push_back('\r');
    container.push_back('\n');
    for(int i = 0; i<data.size(); i++)
    {
        container.push_back(data[i]);        
    }
    container.push_back('\r');
    container.push_back('\n');
    container.push_back('\r');
    container.push_back('\n');
}

void Client::menu()
{
    std::string command;
    boost::asio::ip::tcp::socket socket(ioc);
    for(;;)
    {
        std::cout<<"Client$ "<<std::flush;
        std::getline(std::cin, command);
        std::cout<<std::endl<<command<<std::endl;
        if(command == "help")
        {
            help();
        }
        if(command == "connect")
        {
            connect(socket);
        }
        if(command == "send")
        {
            send(socket);
        }
        if(command == "exit")
        {
            exitSession(socket);
        }
        else
        {
            std::cout<<"Usage: <command>. Go to help command to see info"<<std::endl;
        }
    }
}
void Client::connect(boost::asio::ip::tcp::socket& sock)
{ 
    boost::system::error_code ec;
    //boost::asio::ip::tcp::endpoint address(boost::asio::ip::address::from_string("127.0.0.1"), port);
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::resolver::results_type endpoints = 
                            resolver.resolve(host, std::to_string(port));
    //boost::asio::ip::tcp::socket socket(ioc);
    boost::asio::connect(sock, endpoints);
    // if(ec)
    // {
    //     std::cerr<<ec<<std::endl;
    // }
    // for(;;)
    // {
    //     //std::getline(std::cin, data);
    //     createMessage(message, data);
    //     boost::asio::write(socket, boost::asio::buffer(message));
    //     //socket.write_some(boost::asio::buffer(message));   
    // }
    // socket.shutdown(boost::asio::socket_base::shutdown_both);
    // socket.close();
}

void Client::send(boost::asio::ip::tcp::socket& sock)
{
    createMessage(message, data);
    boost::asio::write(sock, boost::asio::buffer(message));
}

void Client::exitSession(boost::asio::ip::tcp::socket& sock)
{
    sock.close();
    exit(0);
}

void Client::help()
{
    std::string info = "Usage: <command>\nCommands:";
    info += "\n\thelp --> This message\n";
    info += "\n\tconnect --> Connect to open socket\n";
    info += "\n\tsend <message>--> send message to open socket\n";
    info += "\n\texit --> close slient session\n";
    std::cout<<info<<std::flush;
}