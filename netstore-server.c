#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "err.h"

#define MAX_SPACE 52428800
#define TIMEOUT 5

struct parameters {
  char *mcast_addr;
  in_port_t cmd_port;
  unsigned long long max_space;
  char *shrd_fldr;
  unsigned int timeout;
};

bool parse_arg(int argc, char **argv, struct parameters *parameters) {
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
        timeout = (int) atoi(optarg);
        if (timeout < 0 || timeout > 300)
          return false;
        parameters->timeout = timeout;
        break;
      case '?':
        printf("unknown parameter\n");
        return false;
    }
  }

  if (g == 0 || p == 0 || f == 0)
    return false;
  return true;
}

int main(int argc, char *argv[]) {
  /* structure for arguments */
  struct parameters parameters;
  parameters.max_space = MAX_SPACE;
  parameters.timeout = TIMEOUT;

  /* variables and structures describing socket */
  int sock;
  struct sockaddr_in local_addr;
  struct ip_mreq ip_mreq;

  /* parsing program's arguments */
  bool ret = parse_arg(argc, argv, &parameters);
  printf("%s\n", ret ? "true" : "false");
  if (ret)
    printf("%s, %d, %lld, %s, %d\n", parameters.mcast_addr, parameters.cmd_port, parameters.max_space, parameters.shrd_fldr, parameters.timeout);

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

  /* attach to local address and port */
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(parameters.cmd_port);
  if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    syserr("bind");


  return 0;
}