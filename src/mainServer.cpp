#include "server.hpp"

int main() {
  Server server;
  server.setPort(1111);
  server.menu();
  return 0;
}