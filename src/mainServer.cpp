#include "server.hpp"
#define PORT 9999
#define HOST "127.0.0.1"
int main() {
  Server server(PORT, HOST);  // NOLINT
  server.setPort(PORT - 2);
  server.menu();
  return 0;
}