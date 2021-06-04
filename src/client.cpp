#include "client.hpp"

#define BASE 10
static bool is_connected = false;

Client::Client(int p, std::string h, std::string d) noexcept
    : _port(p), _host(std::move(h)), _data(std::move(d)) {
      _port = p;
    }
Client::Client(Client&& other) noexcept {
  _port = other._port;
  _host = other._host;
  _data = other._data;
  for (auto& elem : other._message) {
    _message.push_back(elem);
  }
  other._port = -1;
  other._data.clear();
  other._host.clear();
  other._message.clear();

}
Client& Client::operator=(Client&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  this->_port = other._port;
  this->_host = other._host;
  this->_data = other._data;
  for (auto& elem : other._message) {
    this->_message.push_back(elem);
  }
  other._port = -1;
  other._data.clear();
  other._host.clear();
  other._message.clear();
  return *this;
}
Client::~Client() { ioc.stop(); }

void Client::createMessage(std::vector<char>* container,
                           const std::string& data) {
  _message.clear();
  std::string header = "Content-Lenght: " + std::to_string(data.size());
  for (const auto& i : header) {
    container->push_back(i);
  }
  container->push_back('\r');
  container->push_back('\n');
  for (const auto& i : data) {
    container->push_back(i);
  }
  container->push_back('\r');
  container->push_back('\n');
  container->push_back('\r');
  container->push_back('\n');
}

void Client::menu() {
  std::string command;
  // NOLINTNEXTLINE
  boost::asio::ip::tcp::socket socket(ioc);
  for (;;) {
    std::cout << "Client$ " << std::flush;
    std::getline(std::cin, command);
    if (command == "help") {
      help();
    }
    if (command == "connect") {
      std::cout << "\tEstablishing connection" << std::endl;
      connect(&socket);
    }
    if (command == "send") {
      send(socket);
    }
    if (command == "exit") {
      std::cout << "\tExit yout session" << std::endl;
      exitSession(&socket);
    }
    if ((command != "help") && (command != "connect") && (command != "send") &&
        (command != "exit")) {
      std::cout << "\tUsage: <command>. Go to <help> command to see info"
                << std::endl;
    }
  }
}
void Client::connect(boost::asio::ip::tcp::socket* sock) {
  fetchPort();
  // NOLINTNEXTLINE
  boost::system::error_code ec;
  // NOLINTNEXTLINE
  boost::asio::ip::tcp::resolver resolver(ioc);
  // NOLINTNEXTLINE
  boost::asio::ip::tcp::resolver::results_type endpoints =
      resolver.resolve(_host, std::to_string(_port));
  try {
    if (!is_connected) {
      is_connected = true;
      // NOLINTNEXTLINE
      boost::asio::connect(*sock, endpoints);
      std::cout << "\tConnected\n\tReady to send message" << std::endl;
    } else {
      std::cout << "\tAlready connected" << std::endl;
    }

  } catch (const std::exception& e) {
    std::cout << "\tCannot connect. Check if server is up" << std::endl;
    return;
  }
}

void Client::send(boost::asio::ip::tcp::socket& sock) {
  std::cout << "Message: ";
  std::getline(std::cin, _data);
  createMessage(&_message, _data);
  try {
    boost::asio::write(sock, boost::asio::buffer(_message));// NOLINT
    std::cout << "\tSent" << std::endl;

  } catch (std::exception& ex) {
    std::cout << "Error occured. Check if your connect to open socket"
              << std::endl;
  }
}

void Client::exitSession(boost::asio::ip::tcp::socket* sock) {
  // NOLINTNEXTLINE
  sock->shutdown(boost::asio::ip::tcp::socket::shutdown_send);
  // NOLINTNEXTLINE
  sock->close();
  exit(0);
}

void Client::help() {
  std::string info;
  info += "Usage: <command>\nCommands:";
  info += "\n\thelp --> This message\n";
  info += "\n\tconnect --> Connect to open socket\n";
  info += "\n\tsend <message>--> send message to open socket\n";
  info += "\n\texit --> close slient session\n";
  std::cout << info << std::flush;
}

int Client::fetchPort() {
  std::ifstream port_file;
  std::string p;
  port_file.open("port.txt");// NOLINT
  if (port_file.is_open()) {
    port_file >> p;
  }
  char* p_end;
  _port = static_cast<int>(strtol(p.c_str(), &p_end, BASE));
  // NOLINTNEXTLINE
  port_file.close();
  return _port;
}