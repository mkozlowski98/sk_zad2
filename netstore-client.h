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

  /* class for holding information about available size on particular server from group */
  class Server_Holder {
   public:
    unsigned long long size;
    sockaddr_in addr;
    Server_Holder(unsigned long long, sockaddr_in);
  };

  /* class for holding info about available files from all servers with addresses of each server */
  class File_Info {
   public:
    std::string file;
    sockaddr_in addr;

    File_Info(std::string, sockaddr_in);
  };

  /* comparator for Server_Holder class */
  struct Server_Comparator {
    bool operator()(const Server_Holder&, const Server_Holder&) const;
  };

  /* command line arguments */
  struct client_param parameters;
  /* unique number for client */
  uint64_t cmd_seq;
  /* udp socket to communicate with servers */
  Sock sock;
  /* mutex for displaying info for user */
  std::shared_mutex display_mutex;
  /* keeps all found servers from group */
  std::set<Client::Server_Holder, Client::Server_Comparator> group;
  /* keeps all found files */
  std::vector<Client::File_Info> files;
  /* keeps all created threads */
  std::vector<std::thread> threads;

  /* return size of file specified by argument */
  uint64_t get_size(std::string);
  /* parsing commands from user */
  static std::vector<std::string> get_command();
  /* connect to udp socket, enable broadcasting */
  void connect();
  /* set time for recvfrom while waiting for answer from group of servers */
  template <typename clock>
  void set_recvtime(timeval*, std::chrono::time_point<clock>);
  /* broadcast hello message and save all found servers*/
  void send_discover(bool);
  /* broadcast list message and recieve answers*/
  void send_search(std::string);
  /* save available files */
  void found_files(sockaddr_in, char *);
  /* print available files for user */
  void print_files();
  /* send download request to server */
  void send_fetch(std::string);
  /* download file from server in new thread */
  void download_file(sockaddr_in, unsigned int, std::string, std::string);
  /* send upload request */
  void send_upload(std::string);
  /* upload file to server */
  void upload_file(sockaddr_in, unsigned short, std::string, std::string);
  /* broadcast remove message */
  void send_remove(std::string);
  /* clean and exit */
  void exit();

 public:
  Client(struct client_param, uint64_t);

  /* wait for user's command and call right function */
  void listen();
};

#endif //ZAD2_NETSTORE_CLIENT_H
