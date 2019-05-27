#include "netstore-server.h"
#include "err.h"

Server::Server(struct server_param _parameters): parameters(_parameters), sock() {}

void Server::listen() {
  connect();

  std::cout << sock.sock_no << "\n";

  Simpl_cmd simpl_cmd{};

  struct sockaddr_in addr {};
  std::memset(&addr, 0, sizeof addr);

  receive(sock.sock_no, addr, simpl_cmd);

  std::cout << simpl_cmd.cmd << " " << be64toh(simpl_cmd.cmd_seq) << "\n";

  if (send(sock.sock_no, addr, Cmplx_cmd(cmd_message[1], be64toh(simpl_cmd.cmd_seq), parameters.max_space, parameters.mcast_addr)) < 0)
    syserr("send in server");

  close(sock.sock_no);
}

void Server::connect() {
  sock.attach_to_multicast(parameters.mcast_addr);
  sock.attach_to_port(parameters.cmd_port);
};

int main(int argc, char *argv[]) {
  struct server_param parameters;
  parameters.timeout = TIMEOUT;
  parameters.max_space = MAX_SPACE;

  /* parsing program's arguments */
  if (!server_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-server -g mcast_addr -p port -f path_to_dir [-b max_space -t timeout]\n";
    fatal("arguments");
  }

  Server server (parameters);
  server.listen();

  exit(EXIT_SUCCESS);
}