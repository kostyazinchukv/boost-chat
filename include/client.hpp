#include <boost/asio.hpp>// NOLINT
#include <iostream>// NOLINT
#include <vector>// NOLINT
#include <cstdlib>// NOLINT
#include <fstream>// NOLINT

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
  void connect(boost::asio::ip::tcp::socket* sock);
  void send(boost::asio::ip::tcp::socket& sock);
  static void exitSession(boost::asio::ip::tcp::socket* sock);
  static void help();

  int fetchPort();

 private:
  int _port;
  std::string _host;
  std::string _data;
  std::vector<char> _message;
  static boost::asio::io_context ioc;
};