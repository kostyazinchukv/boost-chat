#include "../include/server.hpp"
#include <vector>
#include <stdlib.h>

Server::Server()
{
    host = "127.0.0.1";
    port = 8888;
}
Server::~Server()
{
    ioc.stop();
}
void Server::start()
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
        std::cout<<"here"<<std::endl;
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
void Server::setPort(int p)
{
    port = p;
}

