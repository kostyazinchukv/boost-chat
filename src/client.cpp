#include "client.hpp"

#include <stdlib.h>

#include <fstream>

static bool isConnected = false;

Client::Client(int p, std::string h, std::string d)
    : port(p), host(h), data(d) {
  // message.resize(1000);
}

Client::~Client() { ioc.stop(); }

void Client::createMessage(std::vector<char>& container, std::string data) {
  message.clear();
  std::string header = "Content-Lenght: " + std::to_string(data.size());
  for (int i = 0; i < header.size(); i++) {
    container.push_back(header[i]);
  }
  container.push_back('\r');
  container.push_back('\n');
  for (int i = 0; i < data.size(); i++) {
    container.push_back(data[i]);
  }
  container.push_back('\r');
  container.push_back('\n');
  container.push_back('\r');
  container.push_back('\n');
}

void Client::menu() {
  std::string command;
  boost::asio::ip::tcp::socket socket(ioc);
  for (;;) {
    std::cout << "Client$ " << std::flush;
    std::getline(std::cin, command);
    if (command == "help") {
      help();
    }
    if (command == "connect") {
      std::cout << "\tEstablishing connection" << std::endl;
      connect(socket);
    }
    if (command == "send") {
      send(socket);
    }
    if (command == "exit") {
      std::cout << "\tExit yout session" << std::endl;
      exitSession(socket);
    }
    if ((command != "help") && (command != "connect") && (command != "send") &&
        (command != "exit")) {
      std::cout << "\tUsage: <command>. Go to <help> command to see info"
                << std::endl;
    }
  }
}
void Client::connect(boost::asio::ip::tcp::socket& sock) {
  fetchPort();
  boost::system::error_code ec;
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::asio::ip::tcp::resolver::results_type endpoints =
      resolver.resolve(host, std::to_string(port));
  try {
    if (!isConnected) {
      isConnected = true;
      boost::asio::connect(sock, endpoints);
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
  std::getline(std::cin, data);
  createMessage(message, data);
  try {
    boost::asio::write(sock, boost::asio::buffer(message));
    std::cout << "\tSent" << std::endl;

  } catch (std::exception& ex) {
    std::cout << "Error occured. Check if your connect to open socket"
              << std::endl;
  }
}

void Client::exitSession(boost::asio::ip::tcp::socket& sock) {
  sock.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
  sock.close();
  exit(0);
}

void Client::help() {
  std::string info = "Usage: <command>\nCommands:";
  info += "\n\thelp --> This message\n";
  info += "\n\tconnect --> Connect to open socket\n";
  info += "\n\tsend <message>--> send message to open socket\n";
  info += "\n\texit --> close slient session\n";
  std::cout << info << std::flush;
}

int Client::fetchPort() {
  std::ifstream portFile;
  std::string p;
  portFile.open("port.txt");
  if (portFile.is_open()) {
    portFile >> p;
  }
  port = atoi(p.c_str());
  portFile.close();
  return port;
}