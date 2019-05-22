#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <inttypes.h>

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
  struct server_param parameters;
  parameters.max_space = MAX_SPACE;
  parameters.timeout = TIMEOUT;

  /* variables and structures describing socket */
  int sock, optval;
  struct sockaddr_in local_addr;
  struct sockaddr_in client_addr;
  struct ip_mreq ip_mreq;
  socklen_t client_addr_len;

  /* variables for communication */

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
  optval = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof optval) < 0)
    syserr("setsockopt");

  /* attach to local address and port */
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(parameters.cmd_port);
  if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    syserr("bind");

  struct simpl_cmd message;
  ssize_t len, prev_len, remains;

  for (;;) {
    client_addr_len = (socklen_t) sizeof(client_addr);

    prev_len = 0; // number of bytes already in the buffer
    do {
      remains = sizeof(message) - prev_len; // number of bytes to be read
      len = recvfrom(sock, (char *)&message + prev_len, remains, 0, (struct sockaddr *) &client_addr, &client_addr_len);
      if (len < 0)
        syserr("reading from client socket");
      else if (len > 0) {
        printf("read %zd bytes from socket\n", len);
        prev_len += len;

        if (prev_len == sizeof(message)) {
          // we have received a whole structure
          printf("received message %s from seq: %" PRIu64 "\n", message.cmd, be64toh(message.cmd_seq));
          printf("ip address: %s and port num:%d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
          strncpy(message.cmd, "GOOD DAY", 10);
          if (sendto(sock, (void *)&message, prev_len, 0, (struct sockaddr *)&client_addr, client_addr_len) != prev_len)
            syserr("sendto");
          printf("sent");
          prev_len = 0;
        }
      }
    } while (len > 0);


  }

  if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&ip_mreq, sizeof ip_mreq) < 0)
    syserr("setsockopt");

  close(sock);
  exit(EXIT_SUCCESS);
}