#include "messages.h"

Simpl_cmd::Simpl_cmd(const char *const cmd, uint64_t cmd_seq, const char *const data) {
  memset(this->cmd, 0, 10);
  strcpy(this->cmd, cmd);
  this->cmd_seq = htobe64(cmd_seq);
  strncpy(this->data, data, DATA_SIZE);
}

Cmplx_cmd::Cmplx_cmd(const char *const cmd, uint64_t cmd_seq, uint64_t param, const char *const data) {
  memset(this->cmd, 0, 10);
  strcpy(this->cmd, cmd);
  this->cmd_seq = htobe64(cmd_seq);
  this->param = htobe64(param);
  strncpy(this->data, data, DATA_SIZE);
}
