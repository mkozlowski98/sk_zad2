#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
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

  if (g == 0 || p == 0 || o == 0)
    return false;
  return true;
}

int main (int argc, char *argv[]) {
  struct client_param parameters;
  parameters.timeout = TIMEOUT;

  int sock, optval;
  struct sockaddr_in remote_addr;

  struct simpl_cmd message;
  uint64_t cmd_seq = 2;
  ssize_t len;

  if (parse_arg(argc, argv, &parameters))
    printf("%s, %d, %d\n", parameters.mcast_addr, parameters.cmd_port, parameters.timeout);
  else
    printf("arguments' error\n");

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    syserr("socket");

  optval = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt broadcast");

  optval = 5;
  if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt multicast ttl");

  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port = htons(parameters.cmd_port);
  if (inet_aton(parameters.mcast_addr, &remote_addr.sin_addr) == 0)
    syserr("inet_aton");
  if (connect(sock, (struct sockaddr *)&remote_addr, sizeof remote_addr) < 0)
    syserr("connect");


//  message.cmd = malloc(10 * sizeof(char));
//  message.data = malloc(0 * sizeof(char));
  strncpy(message.cmd, "HELLO", 10);
  message.cmd_seq = htobe64(cmd_seq);
  len = sizeof(message);
  printf("%ld\n", len);
  if (write(sock, &message, len) != len)
    syserr("write");

//  free(message.cmd);
//  free(message.data);

  close(sock);
  exit(EXIT_SUCCESS);
}

