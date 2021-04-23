#include "../include/server.hpp"
#include <vector>

namespace requirements
{
    using namespace boost::asio;
    io_context ioc;
    ip::tcp::socket socket(ioc);
}
Server::Server()
{
    address = "127.0.0.1";
    port = 8888;
}
Server::~Server()
{

}
void Server::start()
{
    std::cout.flush();
    
    std::cout<<"Server$ "<<std::flush;
    boost::system::error_code ec;
    std::vector<char> buffer;
    buffer.reserve(2 * 1024);
    boost::asio::ip::tcp::acceptor acc(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    std::cout<<"Server$ ";
    for(;;)
    {
        boost::asio::ip::tcp::socket socket(ioc);
        acc.accept(socket, ec);
        if(ec)
        {
            std::cerr<<ec<<std::endl;
        }
        boost::asio::read(socket, boost::asio::buffer(buffer.data(), buffer.size()));
    }
}
void Server::stop()
{
    requirements::socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}

void Server::setPort(int p)
{
    port = p;
}

