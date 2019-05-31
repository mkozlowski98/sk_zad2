#include <thread>
#include "netstore-server.h"

Server::Server(struct server_param _parameters): parameters(_parameters), sock{SOCK_DGRAM} {}

Server::~Server() {
  files_list.clear();
  files_list.shrink_to_fit();
  close(sock.sock_no);
}

void Server::start_listening() {
  list_files();
  connect();
  std::cout << "Listening\n";
  Cmplx_cmd cmplx_cmd{};
  std::string message;

  struct sockaddr_in addr {};

  for (;;) {
    memset(&addr, 0, sizeof addr);
    memset(&cmplx_cmd, 0, sizeof(cmplx_cmd));
    receive(sock.sock_no, addr, cmplx_cmd);
    message = cmplx_cmd.cmd;
    std::cout << cmplx_cmd.cmd << " " << be64toh(cmplx_cmd.cmd_seq) << " addr: " << inet_ntoa(addr.sin_addr) << "\n";
    if (message == global::cmd_message["HELLO"])
      hello(be64toh(cmplx_cmd.cmd_seq), addr);
    if (message == global::cmd_message["LIST"])
      filtered_files(be64toh(cmplx_cmd.cmd_seq), addr, cmplx_cmd.data);
    else if (message == global::cmd_message["GET"])
      send_file(be64toh(cmplx_cmd.cmd_seq), addr, cmplx_cmd.data);
  }

}

void Server::connect() {
  sock.attach_to_multicast(parameters.mcast_addr);
  sock.attach_to_port(parameters.cmd_port);
}

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
  std::string mcast_addr(parameters.mcast_addr);
  if (send(sock.sock_no, addr, Cmplx_cmd(global::cmd_message["GOOD_DAY"], cmd_seq, parameters.max_space, mcast_addr)) < 0)
    syserr("send in server");
}

void Server::filtered_files(uint64_t cmd_seq, sockaddr_in addr, const char *data) {
  if (!files_list.empty()) {
    std::string reg(data);
    std::string str_to_send{};
    for (auto &it: files_list) {
      if (it.find(reg) != std::string::npos) {
        if (str_to_send.length() + it.length() > UDP_SIZE) {
          if (send(sock.sock_no, addr, Simpl_cmd(global::cmd_message["MY_LIST"], cmd_seq, str_to_send)) < 0)
            syserr("send in server");
          str_to_send.clear();
        }
        str_to_send += it + "\n";
      }
    }
    if (!str_to_send.empty())
      if (send(sock.sock_no, addr, Simpl_cmd(global::cmd_message["MY_LIST"], cmd_seq, str_to_send)) < 0)
        syserr("send in server");
  }

}

void Server::send_file(uint64_t cmd_seq, sockaddr_in addr, const char *data) {
  auto it = std::find(files_list.begin(), files_list.end(), std::string(data));
  if (it == files_list.end()) {
    std::string message("Trying to download non-existent file");
    print_error(addr, &message);
    return;
  }
  Sock send_sock{SOCK_STREAM};
  send_sock.tcp_socket();
  uint64_t port = ntohs(send_sock.local_addr.sin_port);
  std::string data_str(data);
  if (send(sock.sock_no, addr, Cmplx_cmd(global::cmd_message["CONNECT_ME"], cmd_seq, port, data_str)) < 0)
    syserr("send in server");
  std::thread thread(handle_send, std::ref(send_sock), std::string(data), std::ref(parameters.timeout));

  thread.join();
}

void Server::handle_send(Sock &tcp_sock, std::string file, unsigned int &timeout) {
  sockaddr_in addr{};
  socklen_t len;
  int msgsock = 0;

  if (listen(tcp_sock.sock_no, SOMAXCONN) < 0)
    syserr("listen");

  fcntl(tcp_sock.sock_no, F_SETFL, O_NONBLOCK);

  auto time = std::chrono::system_clock::now();
  while (get_diff(time) < timeout) {
    len = sizeof(addr);
    msgsock = accept(tcp_sock.sock_no, (sockaddr *)&addr, &len);
    if (msgsock >= 0) {
      break;
    }
  }
}

void Server::print_error(sockaddr_in addr, std::string *message) {
  std::cout << "[PCKG ERROR] Skipping invalid package from " << inet_ntoa(addr.sin_addr) << ":" << addr.sin_port\
    << "." << *message << std::endl;
}

int main(int argc, char *argv[]) {
  struct server_param parameters {};
  parameters.timeout = TIMEOUT;
  parameters.max_space = MAX_SPACE;

  /* parsing program's arguments */
  if (!server_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-server -g mcast_addr -p port -f path_to_dir [-b max_space -t timeout]\n";
    fatal("arguments");
  }

  Server server (parameters);
//  sigaction sig_handler {};
//  sig_handler.sa_handler = Server::signal_handler;
//  sigemptyset(&sig_handler.sa_mask);
//  sig_handler.sa_flags = 0;
//
//  sigaction(SIGINT, &sig_handler, nullptr);

  server.start_listening();

  exit(EXIT_SUCCESS);
}