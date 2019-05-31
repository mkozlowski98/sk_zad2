#ifndef ZAD2_NETSTORE_CLIENT_H
#define ZAD2_NETSTORE_CLIENT_H

#include <sstream>
#include <vector>
#include <netdb.h>
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

  static std::vector<std::string> get_command();
  void connect();
  template <typename clock>
  void set_recvtime(timeval*, std::chrono::time_point<clock>);
  void send_discover();
  void send_search(std::string);
  void found_files(char *, char *);
  void print_files();
  void send_fetch(std::string);
  static void download_file(std::string, unsigned short, std::string);
  void send_upload(std::string);
  void send_remove(std::string);
};

#endif //ZAD2_NETSTORE_CLIENT_H
