#include <iostream>
#include <boost/asio.hpp>

class Server{
public:
    Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    ~Server();

    void start();
    void stop();

    void setPort(int p);
private: 
    boost::asio::io_context ioc;
    std::string address;
    int port;

};