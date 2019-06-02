#ifndef ZAD2_SOCK_H
#define ZAD2_SOCK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>

#define MULTICAST_TTL 5

/* class which represents socket and holds socket and sockaddr_in address */
class Sock {
 public:
  int sock_no;
  int optval;
  struct sockaddr_in local_addr;

  explicit Sock(int);

  /* attach sock_no to multicast */
  void attach_to_multicast(char *);
  /* attach sock_no to local address on specific port */
  void attach_to_port(unsigned int);
  /* enable broadcasting on sock_no */
  void enable_broadcasting();
  /* set address to given char* for send and attach sock_no to port */
  void set_address(char*, unsigned int);
  /* set address to given sockaddr_in struct and attach sock_no to port */
  void copy_address(sockaddr_in, unsigned int);
  /* set timeout for recvfrom */
  void set_timeout(timeval& timeval);
  /* open tcp socket on port given by operating system */
  void tcp_socket();
};

#endif //ZAD2_SOCK_H
