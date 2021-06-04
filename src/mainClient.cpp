#include "client.hpp"

int main() {
  Client client(9999, "127.0.0.1", "Hello, from client!");
  client.menu();
  return 0;
}