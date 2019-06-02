#ifndef ZAD2_NETSTORE_CLIENT_H
#define ZAD2_NETSTORE_CLIENT_H

#include <sstream>
#include <vector>
#include <set>
#include <netdb.h>
#include "sock.h"
#include "globals.h"
#include "messages.h"

class Client {
 private:

  class Server_Holder {
   public:
    unsigned long long size;
    sockaddr_in addr;
    Server_Holder(unsigned long long, sockaddr_in);
  };

  class File_Info {
   public:
    std::string file;
    sockaddr_in addr;

    File_Info(std::string, sockaddr_in);
  };

  struct Server_Comparator {
    bool operator()(const Server_Holder&, const Server_Holder&) const;
  };

  struct client_param parameters;
  uint64_t cmd_seq;
  Sock sock;
  std::set<Client::Server_Holder, Client::Server_Comparator> group;
  std::vector<Client::File_Info> files;
  std::vector<std::thread> threads;

  uint64_t get_size(std::string);
  static std::vector<std::string> get_command();
  void connect();
  template <typename clock>
  void set_recvtime(timeval*, std::chrono::time_point<clock>);
  void send_discover(bool);
  void send_search(std::string);
  void found_files(sockaddr_in, char *);
  void print_files();
  void send_fetch(std::string);
  static void download_file(sockaddr_in, unsigned short, std::string, std::string);
  void send_upload(std::string);
  static void upload_file(sockaddr_in, unsigned short, std::string, std::string);
  void send_remove(std::string);
  void exit();

 public:
  Client(struct client_param, uint64_t);

  void listen();
};

#endif //ZAD2_NETSTORE_CLIENT_H
