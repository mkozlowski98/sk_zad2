#ifndef ZAD2_NETSTORE_SERVER_H
#define ZAD2_NETSTORE_SERVER_H

#include <filesystem>
#include <iterator>
#include "structures.h"

class Server {
 private:
  struct server_param parameters;
  Sock sock;
  std::vector<std::string> files_list;

 public:
  Server(struct server_param);
  ~Server();

  void listen();
  void connect();
  void list_files();
  void hello(uint64_t, sockaddr_in);
  void files(uint64_t, sockaddr_in);
  void filtered_files(uint64_t, sockaddr_in, const char *);
};

#endif //ZAD2_NETSTORE_SERVER_H
