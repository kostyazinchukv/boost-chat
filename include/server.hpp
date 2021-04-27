#include <iostream>
#include <boost/asio.hpp>

class Server{
public:
    Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    ~Server();

    void menu();
    void start(boost::asio::ip::tcp::socket& sock);
    void stop(boost::asio::ip::tcp::socket& sock);
    void exitSession(boost::asio::ip::tcp::socket& sock);
    void help();
    void setPort(int p);

    void parseIncome(std::vector<char>& message);
private: 
    boost::asio::io_context ioc;
    std::string header;
    std::string body;
    std::string host;
    int port;
};