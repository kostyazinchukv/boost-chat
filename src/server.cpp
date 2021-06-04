#include "server.hpp"

#include <stdlib.h>

#include <algorithm>
#include <fstream>
#include <thread>
#include <vector>

static bool isSocketClose = false;
static bool socketCreated = false;

Server::Server() {
  host_ = "127.0.0.1";
  port_= 8888;
}
Server::~Server() { ioc_.stop(); }

void Server::menu() {
  std::string command;
  std::thread startThread;
  boost::asio::ip::tcp::socket socket(ioc_);
  for (;;) {
    std::cout << "Server$ " << std::flush;
    std::getline(std::cin, command);
    if (command == "help") {
      help();
    } else if (command == "start") {
      startThread = std::thread(&Server::start, this, std::ref(socket));
      startThread.detach();
    } else if (command == "stop") {
      startThread.~thread();
      stop(socket);
    } else if (command == "exit") {
      exitSession(socket);
    } else if (command.find("setport ") != std::string::npos) {
      std::string p = command.substr(command.find(" ") + 1, command.size());
      for (auto& it : p) {
        if (static_cast<int>(it) < 47 || static_cast<int>(it) > 57) {
          std::cout
              << "Usage setport <int>. Check for misspelling and given value"
              << std::endl;
          break;
        }
      }
      if (atoi(p.c_str()) < 1024) {
        std::cout << "System port. Choose value above 1024" << std::endl;
        break;
      } else {
        setPort(atoi(p.c_str()));
      }
    } else {
      std::cout << "\tUsage: <command>. Go to <help> command to see info"
                << std::endl;
    }
  }
}
void Server::start(boost::asio::ip::tcp::socket& sock) {
  if (ioc_.stopped()) {
    ioc_.run();
  }
  std::string buff;
  buff.resize(100);
  boost::system::error_code ec;
  if (socketCreated) {
  } else {
    socketCreated = true;
    boost::asio::ip::tcp::endpoint address(
        boost::asio::ip::address::from_string(host_), port_);
    boost::asio::ip::tcp::acceptor acc(ioc_, address);
    acc.accept(sock);
    if (isSocketClose) {
      acc.close();
      acc.accept(sock);
    }
  }

  for (;;) {
    sock.read_some(boost::asio::buffer(buff), ec);
    if (ec) {
      std::cout << "\n - Reading is stop or error occured\nServer$ "
                << std::flush;
      return;
    }
    std::cout << buff << "Server$ " << std::flush;
  }
}
void Server::stop(boost::asio::ip::tcp::socket& sock) {
  try {
    sock.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
    ioc_.stop();
    isSocketClose = true;
    std::cout << "Closed" << std::endl;
  } catch (const std::exception& e) {
    std::cout << "Error occured while stopping the server." << '\n';
  }
}
void Server::exitSession(boost::asio::ip::tcp::socket& sock) {
  sock.close();
  exit(0);
}
void Server::help() {
  std::string info = "Usage: <command>\nCommands:";
  info += "\n\thelp --> This message\n";
  info += "\n\tstart --> Open socket\n";
  info += "\n\tstop --> stop recieving/sending on socket\n";
  info += "\n\texit --> close server session\n";
  info += "\n\tsetport <int> --> set value to port parametr\n";
  std::cout << info << std::flush;
}
void Server::setPort(int p) {
  port_ = p;
  std::ofstream portFile;
  portFile.open("port.txt");
  if (!portFile.is_open()) {
    std::cout << "Cannot open the file to write port value" << std::endl;
    return;
  } else {
    portFile << std::to_string(port_);
  }
  portFile.close();
}
