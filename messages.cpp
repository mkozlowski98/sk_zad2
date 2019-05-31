#include "messages.h"

Simpl_cmd::Simpl_cmd(std::string& cmd, uint64_t cmd_seq, std::string& data) {
  memset(this->cmd, 0, 11);
  strcpy(this->cmd, cmd.c_str());
  this->cmd_seq = htobe64(cmd_seq);
  strncpy(this->data, data.c_str(), DATA_SIZE);
}

Cmplx_cmd::Cmplx_cmd(std::string& cmd, uint64_t cmd_seq, uint64_t param, std::string& data) {
  memset(this->cmd, 0, 11);
  strcpy(this->cmd, cmd.c_str());
  this->cmd_seq = htobe64(cmd_seq);
  this->param = htobe64(param);
  strncpy(this->data, data.c_str(), DATA_SIZE);
}
