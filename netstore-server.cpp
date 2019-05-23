#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <cstring>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "err.h"
#include "structures.h"

#define MAX_SPACE 52428800
#define TIMEOUT 5

struct server_param {
  char *mcast_addr;
  in_port_t cmd_port;
  unsigned long long max_space;
  char *shrd_fldr;
  unsigned int timeout;
};

bool parse_arg(int argc, char **argv, struct server_param *parameters) {
  int opt, g = 0, p = 0, f = 0;
  int timeout;

  while ((opt = getopt(argc, argv, "g:p:b:f:t:")) != -1) {
    switch (opt) {
      case 'g':
        g = 1;
        parameters->mcast_addr = optarg;
        break;
      case 'p':
        p = 1;
        parameters->cmd_port = (in_port_t) atoi(optarg);
        break;
      case 'b':
        parameters->max_space = (unsigned long long) atoi(optarg);
        break;
      case 'f':
        f = 1;
        parameters->shrd_fldr = optarg;
        break;
      case 't':
        timeout = atoi(optarg);
        if (timeout < 0 || timeout > 300)
          return false;
        parameters->timeout = (unsigned int) timeout;
        break;
      case '?':
        std::cout << "unknown parameter\n";
        return false;
    }
  }

  if (g * f * p == 0)
    return false;
  return true;
}

int open_socket(struct server_param parameters) {
  int sock, optval = 1;
  struct sockaddr_in local_addr;
  struct ip_mreq ip_mreq;

  /* open socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    syserr("socket");

  /* attach to multicast */
  ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (inet_aton(parameters.mcast_addr, &ip_mreq.imr_multiaddr) == 0)
    syserr("inet_atom");
  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&ip_mreq, sizeof(ip_mreq)) < 0)
    syserr("setsockopt");
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof optval) < 0)
    syserr("setsockopt");

  /* attach to local address and port */
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(parameters.cmd_port);
  if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    syserr("bind");

  return sock;
}

void hello(int sock, uint64_t cmd_seq, struct sockaddr_in *client_addr, struct server_param parameters) {
  struct cmplx_cmd cmplx_cmd;
  strncpy(cmplx_cmd.cmd, "GOOD_DAY", 10);
  cmplx_cmd.cmd_seq = cmd_seq;
  cmplx_cmd.param = htobe64(parameters.max_space);
//  cmplx_cmd.data = parameters.mcast_addr;
  ssize_t len = sizeof(cmplx_cmd);
  if (sendto(sock, &cmplx_cmd, len, 0, (struct sockaddr *)client_addr, sizeof client_addr) != len)
    syserr("sendto");
}

int main(int argc, char *argv[]) {
  /* structure for arguments */
  struct server_param parameters;
  parameters.max_space = MAX_SPACE;
  parameters.timeout = TIMEOUT;

  /* variables and structures describing socket */
  int sock;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len;

  /* parsing program's arguments */
  if (!parse_arg(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-server -g mcast_addr -p port -f path_to_dir [-b max_space -t timeout]\n";
    fatal("arguments");
  }

  sock = open_socket(parameters);
  std::cout << "ready\n";

  struct simpl_cmd simpl_cmd;
  struct cmplx_cmd cmplx_cmd;
  memset(&cmplx_cmd.cmd, 0, 10);

  ssize_t rcv_len;
  ssize_t cmplx_len = sizeof(cmplx_cmd);


  for (;;) {
    client_addr_len = (socklen_t) sizeof(client_addr);
    rcv_len = recvfrom(sock, (char *)&cmplx_cmd, cmplx_len, 0, (struct sockaddr *) &client_addr, &client_addr_len);
    if (rcv_len < 0)
      syserr("reading from client socket");
    else {
      std::cout << "received message " << cmplx_cmd.cmd << "from seq: " << be64toh(cmplx_cmd.cmd_seq) << "\n";
      std::cout << "ip address: " << inet_ntoa(client_addr.sin_addr) << "and port num: " << client_addr.sin_port << "\n";
      if (strcpy(cmplx_cmd.cmd, "HELLO")) {
        std::cout << "simpl_cmd\n";
//        hello(sock, cmplx_cmd.cmd_seq, &client_addr, parameters);
        strncpy(simpl_cmd.cmd, "GOOD_DAY", 10);
//        cmplx_cmd.param = htobe64(parameters.max_space);
//  cmplx_cmd.data = parameters.mcast_addr;

        if (sendto(sock, (void *)&simpl_cmd, sizeof(simpl_cmd), 0, (struct sockaddr *)&client_addr, client_addr_len) != cmplx_len)
          syserr("sendto");
        std::cout << "sent\n";
      }

    }
  }

  close(sock);
  exit(EXIT_SUCCESS);
}