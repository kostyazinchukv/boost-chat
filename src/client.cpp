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
        if(command == "help")
        {
            help();
        }
        if(command == "connect")
        {
            std::cout<<"Establishing connection"<<std::endl;
            connect(socket);
            std::cout<<"Connected\nReady to send message"<<std::endl;
        }
        if(command == "send")
        {
            send(socket);
            std::cout<<"Sent"<<std::endl;
        }
        if(command == "exit")
        {
            std::cout<<"Exit yout session"<<std::endl;
            exitSession(socket);
        }
        if((command != "help") && (command != "connect") && (command != "send") && (command != "exit"))
        {
            std::cout<<"Usage: <command>. Go to <help> command to see info"<<std::endl;
        }
    }
}
void Client::connect(boost::asio::ip::tcp::socket& sock)
{ 
    boost::system::error_code ec;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::resolver::results_type endpoints = 
                            resolver.resolve(host, std::to_string(port));
    boost::asio::connect(sock, endpoints);

}

void Client::send(boost::asio::ip::tcp::socket& sock)
{   
    std::cout<<"Message: ";
    std::getline(std::cin, data);
    createMessage(message, data);
    try
    {
        boost::asio::write(sock, boost::asio::buffer(message));
    }
    catch(std::exception& ex)
    {
        std::cout<<"Error occured. Check if your connect to open socket"<<std::endl;
    }
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