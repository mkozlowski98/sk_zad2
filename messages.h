#ifndef ZAD2_MESSAGES_H
#define ZAD2_MESSAGES_H

#include <iostream>
#include <cstring>
#define SIMPL_SIZE 18
#define SIMPL_DATA_SIZE 64489
#define CMPLX_SIZE 26
#define CMPLX_DATA_SIZE 64481
#define UDP_SIZE 65507

/* defined class for sending via udp */
class __attribute__((__packed__)) Simpl_cmd {
 public:
  char cmd[10];
  uint64_t cmd_seq;
  char data[SIMPL_DATA_SIZE];

  Simpl_cmd() = default;
  Simpl_cmd(std::string&, uint64_t, std::string&);
};

class __attribute__((__packed__)) Cmplx_cmd {
 public:
  char cmd[10];
  uint64_t cmd_seq;
  uint64_t param;
  char data[CMPLX_DATA_SIZE];

  Cmplx_cmd() = default;
  Cmplx_cmd(std::string&, uint64_t, uint64_t, std::string&);
};

#endif //ZAD2_MESSAGES_H
