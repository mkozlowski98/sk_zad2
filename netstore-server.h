#ifndef ZAD2_NETSTORE_SERVER_H
#define ZAD2_NETSTORE_SERVER_H

#include "structures.h"

class Server {
 private:
  struct server_param parameters;
  Sock sock;

 public:
  Server(struct server_param);

  void listen();
  void connect();
};

#endif //ZAD2_NETSTORE_SERVER_H
