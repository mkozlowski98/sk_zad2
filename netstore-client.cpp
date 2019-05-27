#include "netstore-client.h"
#include "err.h"

Client::Client(struct client_param parameters, uint64_t _seq): parameters(parameters), cmd_seq(_seq), sock {} {}

void Client::connect() {
  sock.enable_broadcasting();
  sock.set_address(parameters.mcast_addr, parameters.cmd_port);
}

void Client::send_hello() {
  sockaddr_in rec_addr {};
  Cmplx_cmd cmplx_cmd {};
  memset(&rec_addr, 0, sizeof rec_addr);

  if (send(sock.sock_no, sock.local_addr, Simpl_cmd(cmd_message[0], cmd_seq, empty_str)) < 0)
    syserr("send");

  if (receive(sock.sock_no, rec_addr, cmplx_cmd) > 0) {
    std::cout << "Found " << inet_ntoa(rec_addr.sin_addr) << " with message: " << cmplx_cmd.cmd \
      << " with max_space: " << be64toh(cmplx_cmd.param) << ", mcast_addr: " << cmplx_cmd.data << "\n";
  }
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