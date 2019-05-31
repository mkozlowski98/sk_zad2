#ifndef ZAD2_NETSTORE_SERVER_H
#define ZAD2_NETSTORE_SERVER_H

#include <unistd.h>
#include <filesystem>
#include <iterator>
#include "sock.h"
#include "globals.h"
#include "messages.h"
#include "err.h"

class Server {
 private:
  struct server_param parameters;
  Sock sock;
  std::vector<std::string> files_list;

 public:
  Server(struct server_param);
  ~Server();

  void start_listening();
  void connect();
  void list_files();
  void hello(uint64_t, sockaddr_in);
  void filtered_files(uint64_t, sockaddr_in, const char *);
  void send_file(uint64_t, sockaddr_in, const char *);
  static void handle_send (Sock&, std::string, unsigned int&);
  static void print_error(sockaddr_in, std::string*);
};

#endif //ZAD2_NETSTORE_SERVER_H
