#include <iostream>
#include <getopt.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "err.h"
#include "structures.h"

#define TIMEOUT 5

struct client_param {
  char *mcast_addr;
  in_port_t cmd_port;
  char *out_fldr;
  unsigned int timeout;
};

bool parse_arg(int argc, char *argv[], struct client_param *parameters) {
  int opt, g = 0, p = 0, o = 0;
  int timeout;

  while ((opt = getopt(argc, argv, "g:p:o:t:")) != -1) {
    switch (opt) {
      case 'g':
        g = 1;
        parameters->mcast_addr = optarg;
        break;
      case 'p':
        p = 1;
        parameters->cmd_port = (in_port_t) atoi(optarg);
        break;
      case 'o':
        o = 1;
        parameters->out_fldr = optarg;
        break;
      case 't':
        timeout = atoi(optarg);
        if (timeout <= 0 || timeout > 300)
          return false;
        parameters->timeout = (unsigned int) timeout;
        break;
      case '?':
        printf("unknown parameter\n");
        return false;
    }
  }

  if (g * p * o == 0)
    return false;
  return true;
}

int open_socket() {
  int sock, optval = 1;
  struct sockaddr_in local_addr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    syserr("socket");

  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt broadcast");

  optval = 5;
  if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt multicast ttl");

  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(0);
  if (bind(sock, (struct sockaddr *)&local_addr, sizeof local_addr) < 0)
    syserr("bind");

  return sock;
}

int main(int argc, char *argv[]) {
  /* structure for arguments */
  struct client_param parameters;
  parameters.timeout = TIMEOUT;

  /* variables and structures describing socket */
  int sock;
  struct sockaddr_in remote_addr;
  socklen_t remote_addr_len;

  /* variables for sending messages */
  struct simpl_cmd simpl_cmd;
  uint64_t cmd_seq = 2;
  ssize_t len = sizeof(simpl_cmd);
  struct cmplx_cmd cmplx_cmd;

  /* parsing program's arguments */
  if (!parse_arg(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-client -g mcast_addr -p port -o path_to_dir [-t timeout]\n";
    fatal("arguments");
  }

  sock = open_socket();

  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port = htons(parameters.cmd_port);
  if (inet_aton(parameters.mcast_addr, &(remote_addr.sin_addr)) == 0)
    syserr("inet_aton");
  if (connect(sock, (struct sockaddr *)&remote_addr, sizeof remote_addr) < 0)
    syserr("connect");

  strncpy(simpl_cmd.cmd, "HELLO", 10);
  simpl_cmd.cmd_seq = htobe64(cmd_seq);

  remote_addr_len = (socklen_t) sizeof(remote_addr);
  if (sendto(sock, &simpl_cmd, len, 0, (struct sockaddr *)&remote_addr, remote_addr_len) != len)
    syserr("write");

  if (read(sock, (char*)&cmplx_cmd, sizeof(cmplx_cmd)) < 0)
    syserr("reading from client socket");
  std::cout << "received message " << cmplx_cmd.cmd << "from: " <<  be64toh(cmplx_cmd.cmd_seq) << "\n";

  close(sock);
  exit(EXIT_SUCCESS);
}