#include <boost/asio.hpp>
#include <iostream>

class Server {
 public:
  Server() noexcept;
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;
  ~Server();

  void menu();
  void start(boost::asio::ip::tcp::socket& sock);
  void stop(boost::asio::ip::tcp::socket& sock);
  void exitSession(boost::asio::ip::tcp::socket& sock);
  void help();
  void setPort(int p);

 private:
  boost::asio::io_context ioc_;
  std::string host_;
  int port_;
};