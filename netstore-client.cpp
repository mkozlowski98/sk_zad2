#include "netstore-client.h"
#include "err.h"

Client::Client(struct client_param parameters, uint64_t _seq): parameters(parameters), sock(), cmd_seq(_seq) {}

void Client::connect() {
  sock.enable_broadcasting();
  sock.set_address(parameters.mcast_addr, parameters.cmd_port);
}

void Client::send_hello() {
  Simpl_cmd simpl_cmd(cmd_message[0], cmd_seq, empty_str);

  if (send(sock.sock_no, sock.local_addr, simpl_cmd) < 0)
    syserr("send");
}

int main(int argc, char *argv[]) {
  struct client_param parameters;
  parameters.timeout = TIMEOUT;

  /* parsing program's arguments */
  if (!client_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-client -g mcast_addr -p port -o path_to_dir [-t timeout]\n";
    fatal("arguments");
  }

  Client client(parameters, 2);
  client.connect();
  client.send_hello();
}