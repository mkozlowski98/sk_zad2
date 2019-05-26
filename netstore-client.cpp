#include "netstore-client.h"
#include "err.h"

Client::Client(struct client_param parameters) {
  this->parameters = parameters;
}

void Client::connect() {
  sock.enable_broadcasting();
}

int main(int argc, char *argv[]) {
  struct client_param parameters;
  parameters.timeout = TIMEOUT;

  /* parsing program's arguments */
  if (!client_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-client -g mcast_addr -p port -o path_to_dir [-t timeout]\n";
    fatal("arguments");
  }
}