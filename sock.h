#ifndef ZAD2_SOCK_H
#define ZAD2_SOCK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MULTICAST_TTL 5

class Sock {
 public:
  int sock_no;
  int optval;
  struct sockaddr_in local_addr;

  Sock();

  void attach_to_multicast(char *mcast_addr);
  void attach_to_port(in_port_t port);
  void enable_broadcasting();
  void set_address(char* addr, in_port_t port);
  void set_timeout(timeval& timeval);
};

#endif //ZAD2_SOCK_H
