#include "globals.h"

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
        parameters->cmd_port = std::stoi(optarg);
        break;
      case 'b':
        parameters->max_space = (unsigned long long) std::stol(optarg);
        break;
      case 'f':
        f = 1;
        parameters->shrd_fldr = optarg;
        break;
      case 't':
        timeout = std::stoi(optarg);
        if (timeout < 0 || timeout > 300)
          return false;
        parameters->timeout = (unsigned int) timeout * 1000;
        break;
      case '?':
        std::cout << "unknown parameter\n";
        return false;
      default:
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
        parameters->cmd_port = std::stoi(optarg);
        break;
      case 'o':
        o = 1;
        parameters->out_fldr = optarg;
        break;
      case 't':
        timeout = std::stoi(optarg);
        if (timeout <= 0 || timeout > 300)
          return false;
        parameters->timeout = (unsigned int) timeout * 1000;
        break;
      case '?':
        printf("unknown parameter\n");
        return false;
      default:
        return false;
    }
  }

  if (g * p * o == 0)
    return false;
  return true;
}
