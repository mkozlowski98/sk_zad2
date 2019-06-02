#ifndef ZAD2_NETSTORE_SERVER_H
#define ZAD2_NETSTORE_SERVER_H

#include <iterator>
#include "sock.h"
#include "globals.h"
#include "messages.h"
#include "err.h"

class Server {
 private:

  /* class which download or upload file to/from client */
  class Sender {
   private:
    Sock sock;
    Server& parent;
    std::string path;
    std::string file;
    int udp_sock;
    sockaddr_in client_addr;
    uint64_t cmd_seq;

    /* return socket if client connected to it */
    int get_msg_sock();

   public:
    Sender(Server&, std::string, std::string, int, sockaddr_in, uint64_t);
    ~Sender() = default;

    /* attach socket to port and return number */
    uint64_t get_port();
    /* send file to client via tcp connection */
    void send_file();
    /* upload file from client via tcp connection */
    void upload_file();
  };

  /* command line arguments */
  struct server_param parameters;
  /* udp socket */
  Sock sock;
  /* available files */
  std::vector<std::string> files_list;
  /* created threads */
  std::vector<std::thread> threads;
  /* mutex for files */
  std::shared_mutex file_mutex;

  /* attach socket to multicast and port */
  void connect();
  /* remember all files from shrd_fldr */
  void list_files();
  /* answer to hello message */
  void hello(uint64_t, sockaddr_in);
  /* answer to list message - send all available files */
  void send_list(uint64_t, sockaddr_in, char *);
  /* chaeck if given file is available */
  bool file_exist(std::string&, sockaddr_in);
  /* send file to client if exists */
  void send_file(uint64_t, sockaddr_in, char *);
  /* function for thread which sends file */
  void handle_send(Sender);
  /* remove file from shrd_fldr */
  void remove_file(sockaddr_in, char *);
  /* function for thread to remove file */
  void handle_remove(std::string, std::string);
  /* upload file from client */
  void add_file(uint64_t, sockaddr_in, char *, uint64_t);
  /* function for thread which upload file */
  void handle_upload(Sender);
  /* check if file can be uploaded */
  bool check_file(std::string&);
  /* print error message */
  static void print_error(sockaddr_in, std::string*);

 public:
  explicit Server(struct server_param);
  ~Server() = default;

  /* clear after SIGINT and exit */
  void clear();
  /* wait for datagrams from clients and call right function */
  void start_listening();
};

#endif //ZAD2_NETSTORE_SERVER_H
