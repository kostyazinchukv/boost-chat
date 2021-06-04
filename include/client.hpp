#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>   // NOLINT
#include <fstream>   // NOLINT
#include <iostream>  // NOLINT
#include <vector>    // NOLINT

class Client {
 public:
  Client() = default;
  Client(int p, std::string h, std::string d) noexcept;
  Client(const Client& other) = delete;
  Client(Client&& other) noexcept;
  Client& operator=(const Client& other) = delete;
  Client& operator=(Client&& other) noexcept;
  ~Client();

  void createMessage(std::vector<char>* container, const std::string& data);
  void menu();
  void connect();
  void send();
  void exitSession();
  static void help();

  int fetchPort();

 private:
  int _port{};
  std::string _host;
  std::string _data;
  std::vector<char> _message;
  boost::asio::io_context _ioc;
  std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
};

#endif // CLIENT_HPP
