#include "server.hpp"


#define SYSTEM_PORTS_VALUES 1024
#define DECIMAL_ASCII_LOWER_BOUND 47
#define DECIMAL_ASCII_UPPER_BOUND 57
#define PORT 9999
#define BASE 10
#define RESIZE_AMOUNT 100

static bool is_socket_close = false;
static bool socket_created = false;

Server::Server(int port, std::string host) noexcept
  : _port(port), _host(std::move(host)){
  _socket = std::make_unique<boost::asio::ip::tcp::socket>(_ioc);
}
Server::Server(Server&& other) noexcept
{
 _port = other._port;
 _host = other._host;
 _socket = std::move(other._socket);

 other._port = -1;
 other._host.clear();
}
Server& Server::operator=(Server&& other) noexcept
{
  if(this == &other)
  {
    return *this;
  }
 _port = other._port;
 _host = other._host;
 _socket = std::move(other._socket);

 other._port = -1;
 other._host.clear();

 return *this;
}

Server::~Server() { _ioc.stop(); }

void Server::menu() {
  char *p_end;
  std::string command;
  std::thread start_thread;// NOLINT
  boost::asio::ip::tcp::socket socket(_ioc);// NOLINT
  for (;;) {
    std::cout << "Server$ " << std::flush;
    std::getline(std::cin, command);
    if (command == "help") {
      help();
    } else if (command == "start") {
      start_thread = std::thread(&Server::start, this, std::ref(socket));// NOLINT
      start_thread.detach();// NOLINT
    } else if (command == "stop") {
      start_thread.~thread();// NOLINT
      stop();
    } else if (command == "exit") {
      exitSession();
      //NOLINTNEXTLINE
    } else if (command.find("setport ") != std::string::npos) {
      //NOLINTNEXTLINE
      std::string p = command.substr(command.find(" ") + 1, command.size());
      for (auto& it : p) {
        if (static_cast<int>(it) < DECIMAL_ASCII_LOWER_BOUND || static_cast<int>(it) > DECIMAL_ASCII_UPPER_BOUND) {
          std::cout
              << "Usage setport <int>. Check for misspelling and given value"
              << std::endl;
          break;
        }
      }
      if (strtol(p.c_str(), &p_end, BASE) < SYSTEM_PORTS_VALUES ) {
        std::cout << "System port. Choose value above 1024" << std::endl;
        break;
      } 
      setPort(static_cast<int>(strtol(p.c_str(), &p_end, BASE)));

    } else {
      std::cout << "\tUsage: <command>. Go to <help> command to see info"
                << std::endl;
    }
  }
}
void Server::start() {
  if (_ioc.stopped()) {
    _ioc.run();
  }
  std::string buff;
  buff.resize(RESIZE_AMOUNT);
  boost::system::error_code ec;
  if (socket_created) {
  } else {
    socket_created = true;
    // NOLINTNEXTLINE
    boost::asio::ip::tcp::endpoint address(
                        boost::asio::ip::address::from_string(_host), _port);// NOLINT
    boost::asio::ip::tcp::acceptor acc(_ioc, address);// NOLINT
    acc.accept(*_socket);// NOLINT
    if (is_socket_close) {
      acc.close();// NOLINT
      acc.accept(*_socket);// NOLINT
    }
  }

  for (;;) {
    _socket->read_some(boost::asio::buffer(buff), ec);
    if (ec) {
      std::cout << "\n - Reading is stop or error occured\nServer$ "
                << std::flush;
      return;
    }
    std::cout << buff << "Server$ " << std::flush;
  }
}
void Server::stop() {
  try {
    _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_receive);// NOLINT
    _ioc.stop();
    is_socket_close = true;
    std::cout << "Closed" << std::endl;
  } catch (const std::exception& e) {
    std::cout << "Error occured while stopping the server." << '\n';
  }
}
void Server::exitSession() {
  _socket->close();
  exit(0);
}
void Server::help() {
  std::string info;
  info += "Usage: <command>\nCommands:";
  info += "\n\thelp --> This message\n";
  info += "\n\tstart --> Open socket\n";
  info += "\n\tstop --> stop recieving/sending on socket\n";
  info += "\n\texit --> close server session\n";
  info += "\n\tsetport <int> --> set value to port parametr\n";
  std::cout << info << std::flush;
}
void Server::setPort(int p) {
  _port = p;
  std::ofstream port_file;
  port_file.open("port.txt");// NOLINT
  if (!port_file.is_open()) {
    std::cout << "Cannot open the file to write port value" << std::endl;
    return;
  }
  port_file << std::to_string(_port);
  port_file.close();// NOLINT
}


