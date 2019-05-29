#ifndef ZAD2_MESSAGES_H
#define ZAD2_MESSAGES_H

#include <iostream>
#include <cstring>
#define DATA_SIZE 1000

class __attribute__((__packed__)) Simpl_cmd {
 public:
  char cmd[10];
  uint64_t cmd_seq;
  char data[DATA_SIZE];

  Simpl_cmd() = default;
  Simpl_cmd(std::string, uint64_t, std::string);
};

class __attribute__((__packed__)) Cmplx_cmd {
 public:
  char cmd[10];
  uint64_t cmd_seq;
  char data[DATA_SIZE];
  uint64_t param;

  Cmplx_cmd() = default;
  Cmplx_cmd(std::string, uint64_t, uint64_t, std::string);
};

#endif //ZAD2_MESSAGES_H
