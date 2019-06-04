#ifndef ZAD2_GLOBALS_H
#define ZAD2_GLOBALS_H

#include <signal.h>
#include <getopt.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <mutex>
#include <shared_mutex>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <chrono>

#define TIMEOUT 5000
#define MAX_SPACE 52428800
#define BUFF_SIZE 1024

namespace global {
  static std::string empty_str {};
  static std::map<std::string, std::string> cmd_message = {
      {"HELLO", "HELLO"}, {"GOOD_DAY", "GOOD_DAY"},
      {"LIST", "LIST"}, {"MY_LIST", "MY_LIST"},
      {"GET", "GET"}, {"CONNECT_ME", "CONNECT_ME"},
      {"DEL", "DEL"}, {"ADD", "ADD"}, {"NO_WAY", "NO_WAY"},
      {"CAN_ADD", "CAN_ADD"}};
  extern bool flag;
}

/* struct for parameters for server */
struct server_param {
  char *mcast_addr;
  short cmd_port;
  unsigned long long max_space;
  char *shrd_fldr;
  unsigned int timeout;
};

/* struct for parameters for client*/
struct client_param {
  char *mcast_addr;
  short cmd_port;
  char *out_fldr;
  unsigned int timeout;
};

/* function to parse program argumnets for server and client */
bool server_parse(int, char **, struct server_param *);
bool client_parse(int, char **, struct client_param *);


/* receive datagram and save it on T object */
template <typename T>
ssize_t receive(int sock, struct sockaddr_in &addr, T& cmd) {
  socklen_t rcva_len;
  rcva_len = (socklen_t) sizeof(addr);
  return recvfrom(sock, (char *)&cmd, sizeof cmd, 0, (struct sockaddr *)&addr, &rcva_len);
}

/* send datagram with T object to address addr */
template <typename T>
ssize_t send(int sock, struct sockaddr_in addr, T cmd, size_t len) {
  socklen_t rcva_len;
  rcva_len = (socklen_t) sizeof(addr);
  return sendto(sock, (char *)&cmd, len, 0, (struct sockaddr *)&addr, rcva_len);
}

/* return how many milliseconds passed since time */
template <typename clock>
unsigned int get_diff(std::chrono::time_point<clock> time) {
  auto time_now = std::chrono::system_clock::now();
  auto millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time).count();
  return millisecs;
}

/* SIGINT handler for server */
void signal_handler(int);
#endif //ZAD2_GLOBALS_H
