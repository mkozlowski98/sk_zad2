#include "netstore-server.h"
#include "err.h"

Server::Server(struct server_param _parameters): parameters(_parameters), sock() {}

Server::~Server() {
  files_list.clear();
  files_list.shrink_to_fit();
  close(sock.sock_no);
}

void Server::listen() {
  list_files();

  connect();

  std::cout << "Listening\n";

  Cmplx_cmd cmplx_cmd{};

  struct sockaddr_in addr {};

  for (;;) {
    memset(&addr, 0, sizeof addr);
    memset(&cmplx_cmd, 0, sizeof(cmplx_cmd));
    receive(sock.sock_no, addr, cmplx_cmd);
    std::cout << cmplx_cmd.cmd << " " << be64toh(cmplx_cmd.cmd_seq) << " addr: " << inet_ntoa(addr.sin_addr) << "\n";
    if (strcmp(cmplx_cmd.cmd, cmd_message[0]) == 0)
      hello(be64toh(cmplx_cmd.cmd_seq), addr);
    if (strcmp(cmplx_cmd.cmd, cmd_message[2]) == 0) {
      if (strcmp(cmplx_cmd.data, empty_str) == 0)
        files(be64toh(cmplx_cmd.cmd_seq), addr);
      else
        filtered_files(be64toh(cmplx_cmd.cmd_seq), addr, cmplx_cmd.data);
    }
  }

}

void Server::connect() {
  sock.attach_to_multicast(parameters.mcast_addr);
  sock.attach_to_port(parameters.cmd_port);
};

void Server::list_files() {
  namespace fs = std::filesystem;

  fs::path path {parameters.shrd_fldr};

  for (auto& entry: fs::directory_iterator(path)) {
    if (fs::is_regular_file(entry)) {
      files_list.push_back(entry.path().filename().string());
    }
  }
}

void Server::hello(uint64_t cmd_seq, sockaddr_in addr) {
  if (send(sock.sock_no, addr, Cmplx_cmd(cmd_message[1], cmd_seq, parameters.max_space, parameters.mcast_addr)) < 0)
    syserr("send in server");
}

void Server::files(uint64_t cmd_seq, sockaddr_in addr) {
  if (!files_list.empty()) {
    std::ostringstream files;
    std::copy(files_list.begin(), files_list.end(), std::ostream_iterator<std::string>(files, "\n"));
    if (send(sock.sock_no, addr, Simpl_cmd(cmd_message[3], cmd_seq, files.str().c_str())) < 0)
      syserr("send in server");
  }
}

void Server::filtered_files(uint64_t cmd_seq, sockaddr_in addr, const char *data) {
  if (!files_list.empty()) {
    std::vector<std::string> filtered;
    for (auto it = files_list.begin(); it != files_list.end(); ++it)
      if (strstr((*it).c_str(), data) != nullptr)
        filtered.push_back((*it));
    std::ostringstream files;
    std::copy(filtered.begin(), filtered.end(), std::ostream_iterator<std::string>(files, "\n"));
    if (send(sock.sock_no, addr, Simpl_cmd(cmd_message[3], cmd_seq, files.str().c_str())) < 0)
      syserr("send in server");
  }

}
int main(int argc, char *argv[]) {
  struct server_param parameters;
  parameters.timeout = TIMEOUT;
  parameters.max_space = MAX_SPACE;

  /* parsing program's arguments */
  if (!server_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-server -g mcast_addr -p port -f path_to_dir [-b max_space -t timeout]\n";
    fatal("arguments");
  }

  Server server (parameters);
  struct sigaction sig_handler;
  sig_handler.sa_handler = signal_handler;
  sigemptyset(&sig_handler.sa_mask);
  sig_handler.sa_flags = 0;

  sigaction(SIGINT, &sig_handler, NULL);

  server.listen();

  exit(EXIT_SUCCESS);
}