#ifndef ZAD2_NETSTORE_CLIENT_H
#define ZAD2_NETSTORE_CLIENT_H

#include "structures.h"

class Client {
 private:
  struct client_param parameters;
  uint64_t cmd_seq;
  Sock sock;

 public:
  Client(struct client_param, uint64_t);

  void connect();
  void send_hello();
};

#endif //ZAD2_NETSTORE_CLIENT_H
