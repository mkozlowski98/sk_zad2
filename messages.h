#ifndef ZAD2_MESSAGES_H
#define ZAD2_MESSAGES_H

#include <iostream>
#include <cstring>
#define SIMPL_DATA_SIZE 64489
#define CMPLX_DATA_SIZE 64481

/* defined class for sending via udp */
class __attribute__((__packed__)) Simpl_cmd {
 public:
  char cmd[11];
  uint64_t cmd_seq;
  char data[SIMPL_DATA_SIZE];

  Simpl_cmd() = default;
  Simpl_cmd(std::string&, uint64_t, std::string&);
};

class __attribute__((__packed__)) Cmplx_cmd {
 public:
  char cmd[11];
  uint64_t cmd_seq;
  char data[CMPLX_DATA_SIZE];
  uint64_t param;

  Cmplx_cmd() = default;
  Cmplx_cmd(std::string&, uint64_t, uint64_t, std::string&);
};

#endif //ZAD2_MESSAGES_H
