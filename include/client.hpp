#include <iostream> 
#include <vector>
#include <boost/asio.hpp>

class Client{
    public:
        Client() = default;
        Client(int p, std::string h, std::string d="");
        ~Client();

        void createMessage(std::vector<char>& containter, std::string data);
        void menu();
        void connect(boost::asio::ip::tcp::socket& sock);
        void send(boost::asio::ip::tcp::socket& sock);
        void exitSession(boost::asio::ip::tcp::socket& sock);
        void help();

        int fetchPort();
    private:
        int port;
        std::string host;
        std::string data;
        std::vector<char> message;
        boost::asio::io_context ioc;

};