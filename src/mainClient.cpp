#include "client.hpp"
#define PORT 9999
int main() {
  int port = PORT;
  Client client(port, "127.0.0.1" /* NOLINT*/,
                "Hello, from client!" /* NOLINT*/);
  client.menu();
  return 0;
}