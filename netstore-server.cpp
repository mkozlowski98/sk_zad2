#include "netstore-server.h"

Server::Sender::Sender(Server& _parent, std::string _path): sock{SOCK_STREAM}, parent(_parent), path(std::move(_path)) {}

int Server::Sender::get_msg_sock() {
  int msgsock = -1;
  sockaddr_in addr{};
  socklen_t len;
  fcntl(sock.sock_no, F_SETFL, O_NONBLOCK);

  auto time = std::chrono::system_clock::now();
  while (get_diff(time) < parent.parameters.timeout && msgsock == -1) { // wait for connection from client
    len = sizeof(addr);
    msgsock = accept(sock.sock_no, (sockaddr *)&addr, &len);
  }

  return msgsock;
}

uint64_t Server::Sender::get_port() {
  sock.tcp_socket();
  return ntohs(sock.local_addr.sin_port);
}

void Server::Sender::send_file() {
  if (listen(sock.sock_no, SOMAXCONN) < 0)
    syserr("listen");

  int msgsock = get_msg_sock();
  close(sock.sock_no);

  if (msgsock > 0) { //connection established, send file
    std::fstream fd(path.c_str(), std::ios::in);
    char *buffer;
    buffer = (char *) malloc(BUFF_SIZE * sizeof(char));
    if (!fd.is_open())
      syserr("fopen");
    else {
      while (!fd.eof()) {
        memset(buffer, 0, 1024);
        fd.read(buffer, 1024);
        if (send(msgsock, (void *)buffer, fd.gcount(), 0) < 0)
          syserr("send in server");
      }
      free(buffer);
      fd.close();
    }
    close(msgsock);
  }
}

Server::Server(struct server_param _parameters): parameters(_parameters), sock{SOCK_DGRAM} {}

void Server::clear() {
  files_list.clear();
  files_list.shrink_to_fit();
  close(sock.sock_no);
  for (auto &thread: threads) {
    if (thread.joinable())
      thread.join();
  }
  threads.clear();
  threads.shrink_to_fit();
}

void Server::start_listening() {
  list_files();
  connect();
  std::cout << "Listening\n";
  Cmplx_cmd cmplx_cmd{};
  std::string message;

  struct sockaddr_in addr {};

  while (global::flag) {
    memset(&addr, 0, sizeof addr);
    memset(&cmplx_cmd, 0, sizeof(cmplx_cmd));
    receive(sock.sock_no, addr, cmplx_cmd);
    message = cmplx_cmd.cmd;
    std::cout << cmplx_cmd.cmd << " port: " << ntohs(addr.sin_port)<< " addr: " << inet_ntoa(addr.sin_addr) << "\n";
    if (message == global::cmd_message["HELLO"])
      hello(be64toh(cmplx_cmd.cmd_seq), addr);
    if (message == global::cmd_message["LIST"])
      filtered_files(be64toh(cmplx_cmd.cmd_seq), addr, cmplx_cmd.data);
    else if (message == global::cmd_message["GET"])
      send_file(be64toh(cmplx_cmd.cmd_seq), addr, cmplx_cmd.data);
    else if (message == global::cmd_message["DEL"])
      remove_file(addr, cmplx_cmd.data);
    else if (message == global::cmd_message["ADD"])
      add_file(be64toh(cmplx_cmd.cmd_seq), addr, cmplx_cmd.data, be64toh(cmplx_cmd.param));
    else {
      std::string mes("Unknown message");
      print_error(addr, &mes);
    }

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
      files_list.emplace_back(entry.path().filename().string());
    }
  }
}

void Server::hello(uint64_t cmd_seq, sockaddr_in addr) {
  std::string mcast_addr(parameters.mcast_addr);
  if (send(sock.sock_no, addr, Cmplx_cmd(global::cmd_message["GOOD_DAY"], cmd_seq, parameters.max_space, mcast_addr)) < 0)
    syserr("send in server");
}

void Server::filtered_files(uint64_t cmd_seq, sockaddr_in addr, char *data) {
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

bool Server::file_exist(std::string &file, sockaddr_in addr) {
  auto it = std::find(files_list.begin(), files_list.end(), file);
  if (it == files_list.end()) {
    std::string message("Trying to download non-existent file");
    print_error(addr, &message);
    return false;
  }
  return true;
}

void Server::send_file(uint64_t cmd_seq, sockaddr_in addr, char *data) {
  std::string file(data);
  if (!file_exist(file, addr))
    return;

  std::string path = std::string(parameters.shrd_fldr) + std::string(data);
  Sender sender{*this, path}; //create object for sending file

  uint64_t port = sender.get_port();
  std::string data_str(data);
  if (send(sock.sock_no, addr, Cmplx_cmd(global::cmd_message["CONNECT_ME"], cmd_seq, port, data_str)) < 0)
    syserr("send in server");

  std::thread thread(&Server::handle_send, this, sender);
  if (thread.joinable()) {
    thread.detach();
    threads.emplace_back(std::move(thread));
  }
}

void Server::handle_send(Sender sender) {
  std::unique_lock lock(this->file_mutex);
  sender.send_file();
}

void Server::remove_file(sockaddr_in addr, char * file) {
  std::string path = std::string(parameters.shrd_fldr) + std::string(file);
  if (std::filesystem::exists(path)) { //check if file exists
    std::fstream fd(path.c_str(), std::ios::in);
    if (fd.is_open()) { //first get size of file
      fd.seekg(0, std::ios::end);
      int file_size = fd.tellg();
      parameters.max_space += file_size;
      fd.close();
      if (remove(path.c_str()) != 0) //remove file
        syserr("remove file");
      else { //remove file from vector
        auto it = std::find(files_list.begin(), files_list.end(), std::string(file));
        files_list.erase(it);
      }
    } else
      syserr("file_size");
  } else {
    std::string message("trying to remove non-existent file");
    print_error(addr, &message);
  }
}

void Server::add_file(uint64_t cmd_seq, sockaddr_in addr, char * file, uint64_t size) {
  std::string path = std::string(parameters.shrd_fldr) + std::string(file);

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
  struct sigaction sig_handler{};
  sig_handler.sa_handler = signal_handler;
  sigemptyset(&sig_handler.sa_mask);
  sig_handler.sa_flags = 0;

  sigaction(SIGINT, &sig_handler, nullptr);

  server.start_listening();
  server.clear();

  exit(EXIT_SUCCESS);
}