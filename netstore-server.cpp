#include "netstore-server.h"
#include "err.h"

Server::Server(struct server_param _parameters): parameters(_parameters), sock() {}

Server::~Server() {
  files_list.clear();
  files_list.shrink_to_fit();
}

void Server::listen() {
  list_files();

  connect();

  std::cout << "Listening\n";

  Cmplx_cmd cmplx_cmd{};

  struct sockaddr_in addr {};
  std::memset(&addr, 0, sizeof addr);

  receive(sock.sock_no, addr, cmplx_cmd);

  std::cout << cmplx_cmd.cmd << " " << be64toh(cmplx_cmd.cmd_seq) << "\n";

  if (strcmp(cmplx_cmd.cmd, cmd_message[0]) == 0)
    hello(be64toh(cmplx_cmd.cmd_seq), addr);

  close(sock.sock_no);
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

//  std::ostringstream files;
//  std::copy(files_list.begin(), files_list.end(), std::ostream_iterator<std::string>(files, "\n"));
//  std::cout << files.str();
}

void Server::hello(uint64_t cmd_seq, sockaddr_in addr) {
  if (send(sock.sock_no, addr, Cmplx_cmd(cmd_message[1], cmd_seq, parameters.max_space, parameters.mcast_addr)) < 0)
    syserr("send in server");
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
  server.listen();
  server.~Server();

  exit(EXIT_SUCCESS);
}