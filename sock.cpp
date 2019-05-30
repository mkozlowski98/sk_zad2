#include "sock.h"
#include "err.h"

Sock::Sock(int type) {
  sock_no = socket(AF_INET, type, 0);
  if (sock_no < 0)
    syserr("socket");
}

void Sock::attach_to_multicast(char *mcast_addr) {
  struct ip_mreq ip_mreq {};
  optval = 1;
  ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (inet_aton(mcast_addr, &ip_mreq.imr_multiaddr) == 0)
    syserr("inet_atom");
  if (setsockopt(sock_no, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&ip_mreq, sizeof ip_mreq) < 0)
    syserr("setsockopt");
  if (setsockopt(sock_no, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof optval) < 0)
    syserr("setsockopt");

  //TODO set timeval for sending
}

void Sock::attach_to_port(in_port_t port) {
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(port);
  if (bind(sock_no, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    syserr("bind");
}

void Sock::enable_broadcasting() {
  optval = 1;
  if (setsockopt(sock_no, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt broadcast");

  optval = MULTICAST_TTL;
  if (setsockopt(sock_no, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt multicast ttl");
}

void Sock::set_address(char *addr, in_port_t port) {
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(port);
  if (inet_aton(addr, &local_addr.sin_addr) == 0)
    syserr("inet_aton");
}

void Sock::set_timeout(timeval &timeout) {
  if (setsockopt(sock_no, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(timeout)) < 0)
    syserr("setsockopt failed");
}

uint64_t Sock::tcp_socket() {
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = 0;
  if (bind(sock_no, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    syserr("bind");
  socklen_t len = sizeof(local_addr);
  if (getsockname(sock_no, (struct sockaddr *)&local_addr, &len) < 0)
    syserr("getsockname");
  return ntohs(local_addr.sin_port);
}