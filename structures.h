#ifndef ZAD2_STRUCTURES_H
#define ZAD2_STRUCTURES_H

#include <cstring>
#include <getopt.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MULTICAST_TTL 5
#define DATA_SIZE 1000
#define TIMEOUT 5
#define MAX_SPACE 52428800

class Sock {
 public:
  int sock_no;
  int optval;
  struct sockaddr_in local_addr;

  Sock();

  void attach_to_multicast(char *mcast_addr);
  void attach_to_port(in_port_t port);
  void enable_broadcasting();
};

class __attribute__((__packed__)) Simpl_cmd {
 public:
  char cmd[10];
  uint64_t cmd_seq;
  char data[DATA_SIZE];

  Simpl_cmd() = default;
  Simpl_cmd(char *, uint64_t, char *);
};

struct server_param {
  char *mcast_addr;
  in_port_t cmd_port;
  unsigned long long max_space;
  char *shrd_fldr;
  unsigned int timeout;
};

struct client_param {
  char *mcast_addr;
  in_port_t cmd_port;
  char *out_fldr;
  unsigned int timeout;
};

bool server_parse(int, char **, struct server_param *);
bool client_parse(int, char **, struct client_param *);

template <typename T>
ssize_t receive(int sock, struct sockaddr_in &addr, T& cmd) {
  socklen_t rcva_len;
  rcva_len = (socklen_t) sizeof(addr);
  return recvfrom(sock, (char *)&cmd, sizeof cmd, 0, (struct sockaddr *)&addr, &rcva_len);
}

template <typename T>
ssize_t send(int sock, struct sockaddr_in addr, T cmd) {
  socklen_t rcva_len;
  rcva_len = (socklen_t) sizeof(addr);
  return sendto(sock, (char *)&cmd, sizeof cmd, 0, (struct sockaddr *)&addr, rcva_len);
}
#endif //ZAD2_STRUCTURES_H
