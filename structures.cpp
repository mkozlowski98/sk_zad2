#include "structures.h"
#include "err.h"

Sock::Sock() {
  sock_no = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_no < 0)
    syserr("socket");
}

void Sock::attach_to_multicast(char *mcast_addr) {
  struct ip_mreq ip_mreq;
  optval = 1;
  ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (inet_aton(mcast_addr, &ip_mreq.imr_multiaddr) == 0)
    syserr("inet_atom");
  if (setsockopt(sock_no, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&ip_mreq, sizeof ip_mreq) < 0)
    syserr("setsockopt");
  if (setsockopt(sock_no, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof optval) < 0)
    syserr("setsockopt");

  //TODO set timeval for sending
}

void Sock::attach_to_port(in_port_t port) {
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(port);
  if (bind(sock_no, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    syserr("bind");
}

void Sock::enable_broadcasting() {
  optval = 1;
  if (setsockopt(sock_no, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt broadcast");

  optval = MULTICAST_TTL;
  if (setsockopt(sock_no, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&optval, sizeof optval) < 0)
    syserr("setsockopt multicast ttl");

  //TODO set timeval for sending and receive
}

void Sock::set_address(char *addr, in_port_t port) {
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(port);
  if (inet_aton(addr, &local_addr.sin_addr) == 0)
    syserr("inet_aton");
}

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

bool server_parse(int argc, char **argv, struct server_param *parameters) {
  int opt, g = 0, p = 0, f = 0;
  int timeout;

  while ((opt = getopt(argc, argv, "g:p:b:f:t:")) != -1) {
    switch (opt) {
      case 'g':
        g = 1;
        parameters->mcast_addr = optarg;
        break;
      case 'p':
        p = 1;
        parameters->cmd_port = (in_port_t) atoi(optarg);
        break;
      case 'b':
        parameters->max_space = (unsigned long long) atoi(optarg);
        break;
      case 'f':
        f = 1;
        parameters->shrd_fldr = optarg;
        break;
      case 't':
        timeout = atoi(optarg);
        if (timeout < 0 || timeout > 300)
          return false;
        parameters->timeout = (unsigned int) timeout;
        break;
      case '?':
        std::cout << "unknown parameter\n";
        return false;
    }
  }

  if (g * f * p == 0)
    return false;
  return true;
}

bool client_parse(int argc, char *argv[], struct client_param *parameters) {
  int opt, g = 0, p = 0, o = 0;
  int timeout;

  while ((opt = getopt(argc, argv, "g:p:o:t:")) != -1) {
    switch (opt) {
      case 'g':
        g = 1;
        parameters->mcast_addr = optarg;
        break;
      case 'p':
        p = 1;
        parameters->cmd_port = (in_port_t) atoi(optarg);
        break;
      case 'o':
        o = 1;
        parameters->out_fldr = optarg;
        break;
      case 't':
        timeout = atoi(optarg);
        if (timeout <= 0 || timeout > 300)
          return false;
        parameters->timeout = (unsigned int) timeout;
        break;
      case '?':
        printf("unknown parameter\n");
        return false;
    }
  }

  if (g * p * o == 0)
    return false;
  return true;
}

void signal_handler(int s) {
  std::cout << "Caught signal " << s << std::endl;
  exit(EXIT_SUCCESS);
}