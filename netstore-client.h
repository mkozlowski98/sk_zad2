#ifndef ZAD2_NETSTORE_CLIENT_H
#define ZAD2_NETSTORE_CLIENT_H

#include <chrono>
#include <algorithm>
#include <sstream>
#include <vector>
#include "sock.h"
#include "globals.h"
#include "messages.h"

class Client {
 private:
  struct client_param parameters;
  uint64_t cmd_seq;
  Sock sock;
  std::vector<std::pair<std::string, std::string>> files;

 public:
  Client(struct client_param, uint64_t);

  void connect();
  template <typename clock>
  unsigned int get_diff(std::chrono::time_point<clock>);
  template <typename clock>
  void set_recvtime(timeval*, std::chrono::time_point<clock>);
  void send_discover();
  void send_search(std::string);
  std::vector<std::string> get_command();
  void found_files(char *, char *);
  void print_files();
  void send_fetch(std::string);
};

#endif //ZAD2_NETSTORE_CLIENT_H
