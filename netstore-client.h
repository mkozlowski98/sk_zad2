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
  template <typename clock>
  unsigned int get_diff(std::chrono::time_point<clock>);
  template <typename clock>
  void set_recvtime(timeval*, std::chrono::time_point<clock>);
  void send_hello();
  void send_list(const char *const);
};

#endif //ZAD2_NETSTORE_CLIENT_H
