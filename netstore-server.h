#ifndef ZAD2_NETSTORE_SERVER_H
#define ZAD2_NETSTORE_SERVER_H

#include <iterator>
#include "sock.h"
#include "globals.h"
#include "messages.h"
#include "err.h"

class Server {
 private:

  class Sender {
   private:
    Sock sock;
    Server& parent;
    std::string path;

    int get_msg_sock();

   public:
    Sender(Server&, std::string);
    ~Sender() = default;

    uint64_t get_port();
    void send_file();
  };

  struct server_param parameters;
  Sock sock;
  std::vector<std::string> files_list;
  std::vector<std::thread> threads;
  std::shared_mutex file_mutex;

  void connect();
  void list_files();
  void hello(uint64_t, sockaddr_in);
  void filtered_files(uint64_t, sockaddr_in, char *);
  bool file_exist(std::string&, sockaddr_in);
  void send_file(uint64_t, sockaddr_in, char *);
  void handle_send (Sender);
  void remove_file(sockaddr_in, char *);
  void add_file(uint64_t, sockaddr_in, char *, uint64_t);
  static void print_error(sockaddr_in, std::string*);

 public:
  explicit Server(struct server_param);
  ~Server() = default;
  void clear();

  void start_listening();
};

#endif //ZAD2_NETSTORE_SERVER_H
