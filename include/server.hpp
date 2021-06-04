#ifndef SERVER_HPP  // NOLINT
#define SERVER_HPP  // NOLINT

#include <algorithm>       // NOLINT
#include <boost/asio.hpp>  // NOLINT
#include <cstdlib>         // NOLINT
#include <fstream>         // NOLINT
#include <iostream>        // NOLINT
#include <thread>          // NOLINT
#include <vector>          // NOLINT

class Server {
 public:
  Server() = default;
  Server(int port, std::string host) noexcept;
  Server(const Server& other) = delete;
  Server& operator=(const Server& other) = delete;
  Server(Server&& other) noexcept;
  Server& operator=(Server&& other) noexcept;
  ~Server();

  void menu();
  void start();
  void stop();
  void exitSession();
  void setPort(int p);
  static void help();

 private:
  int _port{};
  std::string _host;
  boost::asio::io_context _ioc;
  std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
};
#endif  // SERVER_HPP