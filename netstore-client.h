#ifndef ZAD2_NETSTORE_CLIENT_H
#define ZAD2_NETSTORE_CLIENT_H

#include "structures.h"

class Client {
 private:
  struct client_param parameters;
  Sock sock;

 public:
  Client(struct client_param);

  void connect();
};

#endif //ZAD2_NETSTORE_CLIENT_H
