#include "netstore-client.h"
#include "err.h"

Client::Server_Holder::Server_Holder(unsigned long long _size, sockaddr_in _addr): size(_size), addr(_addr) {}

Client::File_Info::File_Info(std::string _file, sockaddr_in _addr): file(std::move(_file)), addr(_addr) {}

bool Client::Server_Comparator::operator()(const Client::Server_Holder & server1, const Client::Server_Holder &server2) const {
  return server1.size >= server2.size;
}

Client::Client(struct client_param parameters, uint64_t _seq): parameters(parameters), cmd_seq(_seq), sock {SOCK_DGRAM} {}

void Client::listen() {
  std::vector<std::string> line;
  bool exit = false;

  connect();

  while (!exit) {
    line = get_command();
    if (line.size() == 2) {
      if (line[0] == "search")
        send_search(line[1]);
      if (line[0] == "fetch")
        send_fetch(line[1]);
      if (line[0] == "upload")
        send_upload(line[1]);
      if (line[0] == "remove")
        send_remove(line[1]);
    } else if (line.size() == 1) {
      if (line[0] == "discover")
        send_discover(true);
      if (line[0] == "search")
        send_search(global::empty_str);
      if (line[0] == "exit") {
        this->exit();
        exit = true;
      }
    }
  }
}

uint64_t Client::get_size(std::string path) {
  std::fstream fd(path.c_str(), std::ios::in);
  uint64_t size = 0;
  if (fd.is_open()) {
    fd.seekg(0, std::ios::end);
    size = fd.tellg();
    fd.close();
  }
  return size;
}

std::vector<std::string> Client::get_command() {
  std::vector<std::string> line;
  std::string command;
  getline(std::cin, command);
  std::transform(command.begin(), command.end(), command.begin(), ::tolower);
  std::stringstream stream(command);
  while (getline(stream, command, ' ')) {
    line.push_back(command);
  }
  return line;
}

void Client::connect() {
  sock.enable_broadcasting();
  sock.set_address(parameters.mcast_addr, parameters.cmd_port);
}

template <typename clock>
void Client::set_recvtime(timeval *recv_timeout, std::chrono::time_point<clock> time) {
  auto time_now = std::chrono::system_clock::now();
  auto milisecs = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time).count();
  milisecs = parameters.timeout - milisecs;
  recv_timeout->tv_sec = milisecs / 1000;
  recv_timeout->tv_usec = (milisecs % 1000) * 1000;
}

void Client::send_discover(bool show) {
  group.clear();
  sockaddr_in rec_addr {};
  Cmplx_cmd cmplx_cmd {};
  memset(&rec_addr, 0, sizeof rec_addr);
  timeval recv_timeout {};

  if (send(sock.sock_no, sock.local_addr, Simpl_cmd(global::cmd_message["HELLO"], cmd_seq, global::empty_str)) < 0)
    syserr("send");

  auto time = std::chrono::system_clock::now();
  /* receive messages from servers until timeout */
  while (get_diff(time) < parameters.timeout) {
    set_recvtime(&recv_timeout, time);
    sock.set_timeout(recv_timeout);
    if (receive(sock.sock_no, rec_addr, cmplx_cmd) > 0) {
      group.insert(Server_Holder{be64toh(cmplx_cmd.param), rec_addr});
      if (show) {
        std::unique_lock lock(display_mutex);
        std::cout << "Found " << inet_ntoa(rec_addr.sin_addr) << " (" << cmplx_cmd.data << ") with free space "\
          << be64toh(cmplx_cmd.param) << std::endl;
      }
    }
  }
}

void Client::send_search(std::string data) {
  files.clear();
  sockaddr_in rec_addr {};
  Simpl_cmd simpl_cmd {};
  memset(&rec_addr, 0, sizeof rec_addr);
  timeval recv_timeout {};

  if (send(sock.sock_no, sock.local_addr, Simpl_cmd(global::cmd_message["LIST"], cmd_seq, data)) < 0)
    syserr("send");

  auto time = std::chrono::system_clock::now();
  /* receive messages from servers until timeout */
  while (get_diff(time) < parameters.timeout) {
    set_recvtime(&recv_timeout, time);
    sock.set_timeout(recv_timeout);
    if (receive(sock.sock_no, rec_addr, simpl_cmd) > 0)
      found_files(rec_addr, simpl_cmd.data);
  }
  print_files();
}

void Client::found_files(sockaddr_in addr, char * data) {
  std::stringstream stream(data);
  std::string file;
  while (getline(stream, file, '\n'))
    files.emplace_back(File_Info{file, addr});
}

void Client::print_files() {
  std::unique_lock lock(display_mutex);
  for (auto &file: files)
    std::cout << file.file << " (" << inet_ntoa(file.addr.sin_addr) << ")" << std::endl;
}

void Client::send_fetch(std::string data) {
  sockaddr_in addr {};
  bool found = false;
  for (auto &file: files) {
    if (file.file == data) {
      addr = file.addr;
      found = true;
      break;
    }
  }

  if (found) {
    Sock fetch_sock {SOCK_DGRAM};
    sockaddr_in rec_addr{};
    Cmplx_cmd cmplx_cmd{};
    fetch_sock.copy_address(addr, ntohs(addr.sin_port));
    timeval timeout{};
    timeout.tv_sec = parameters.timeout / 1000;
    fetch_sock.set_timeout(timeout);
    if (send(fetch_sock.sock_no, fetch_sock.local_addr, Simpl_cmd(global::cmd_message["GET"], cmd_seq, data)) < 0)
      syserr("send");
    if (receive(fetch_sock.sock_no, rec_addr, cmplx_cmd) > 0) {
      close(fetch_sock.sock_no);
      unsigned short port = be64toh(cmplx_cmd.param);
      std::string path = std::string(parameters.out_fldr) + data;
      std::string addr_str = inet_ntoa(rec_addr.sin_addr);
      std::thread thread(&Client::download_file, this, addr_str, port, path, data);
      if (thread.joinable()) {
        thread.detach();
        threads.emplace_back(std::move(thread));
      }
    }
    close(fetch_sock.sock_no);
  } else {
    std::unique_lock lock(display_mutex);
    std::cout << "file doesn't exist" << std::endl;
  }
}

void Client::download_file(std::string addr, unsigned short port, std::string path, std::string file) {
  Sock tcp_sock{SOCK_STREAM};
  tcp_sock.set_address(addr.data(), port);
  std::string addr_str = addr;
  if (::connect(tcp_sock.sock_no, (sockaddr *)&(tcp_sock.local_addr), sizeof(tcp_sock.local_addr)) < 0) {
    std::unique_lock lock(display_mutex);
    std::cout << "File " << file << " downloading failed (" << addr_str << ":" << port << ") error in connect" << std::endl;
  } else {
    std::fstream fd(path.c_str(), std::ios::out);
    /* open file */
    if (fd.is_open()) {
      char *buffer;
      buffer = (char *) malloc(BUFF_SIZE * sizeof(char));
      ssize_t len;
      /* receive whole file from server */
      do {
        if ((len = recv(tcp_sock.sock_no, (void *)buffer, BUFF_SIZE - 1, 0)) < 0) {
          std::unique_lock lock(display_mutex);
          std::cout << "File " << file << " downloading failed (" << addr_str << ":" << port << ") error in read" << std::endl;
        }
        if (len > 0) {
          fd.write(buffer, len);
        }
      } while (len > 0);
      free(buffer);
      fd.close();
      if (len == 0) {
        std::unique_lock lock(display_mutex);
        std::cout << "File " << file << " downloaded (" << addr_str << ":" << port << ")" << std::endl;
      }
    } else {
      std::unique_lock lock(display_mutex);
      std::cout << "File " << file << " downloading failed (" << addr_str << ":" << port << ") couldn't create file" << std::endl;
    }
  }
}

void Client::send_upload(std::string data) {
  std::string path;
  if (data[0] != '/')
    path = parameters.out_fldr + data;
  else
    path = data;
  if (std::filesystem::exists(path)) {
    send_discover(false);
    uint64_t size = get_size(path);
    if (group.begin()->size < size) { //file is too big
      std::unique_lock lock(display_mutex);
      std::cout << "File " << data << " too big" << std::endl;
      return;
    }
    std::thread thread(&Client::choose_server, this, data, path, size);
    if (thread.joinable()) {
      thread.detach();
      threads.emplace_back(std::move(thread));
    }
  } else {
    std::unique_lock lock(display_mutex);
    std::cout << "File " << data << " does not exist" << std::endl;
  }
}

void Client::choose_server(std::string data, std::string path, uint64_t size) {
  Sock upload_sock{SOCK_DGRAM};
  sockaddr_in rec_addr{};
  Cmplx_cmd cmplx_cmd{};
  upload_sock.copy_address(group.begin()->addr, ntohs(group.begin()->addr.sin_port));
  timeval timeout{};
  timeout.tv_sec = parameters.timeout / 1000;
  upload_sock.set_timeout(timeout);
  std::string addr_str;
  bool sent = false;
  for (auto &server: group) {
    addr_str = inet_ntoa(server.addr.sin_addr);
    if (server.size < size) {
      break;
    } else {
      if (send(upload_sock.sock_no, upload_sock.local_addr, Cmplx_cmd(global::cmd_message["ADD"], cmd_seq, size, data)) < 0) {
        std::unique_lock lock(display_mutex);
        std::cout << "File " << data << " uploading failed (" << addr_str << ") error in send" << std::endl;
      }
      if (receive(upload_sock.sock_no, rec_addr, cmplx_cmd) > 0) {
        if (cmplx_cmd.cmd == global::cmd_message["CAN_ADD"]) {
          unsigned short port = be64toh(cmplx_cmd.param);
          if (upload_file(rec_addr, port, path, data)) {
            std::unique_lock lock(display_mutex);
            std::cout << "File " << data << " uploaded (" << addr_str << ":" << port << ")" << std::endl;
            sent = true;
            break;
          }
        }
      }
    }
  }
  if (!sent) {
    std::unique_lock lock(display_mutex);
    std::cout << "File " << data << " uploading failed (" << addr_str << ") none of server could upload file" << std::endl;
  }
  close(upload_sock.sock_no);
}

bool Client::upload_file(sockaddr_in addr, unsigned short port, std::string path, std::string file) {
  Sock tcp_sock{SOCK_STREAM};
  tcp_sock.copy_address(addr, port);
  std::string addr_str = inet_ntoa(addr.sin_addr);
  /* connect to tcp socket opened by server */
  if (::connect(tcp_sock.sock_no, (sockaddr *)&(tcp_sock.local_addr), sizeof(tcp_sock.local_addr)) < 0) {
    std::unique_lock lock(display_mutex);
    std::cout << "File " << file << " uploading failed (" << addr_str << ":" << port << ") error in connect" << std::endl;
    return false;
  } else {
    /* open file */
    std::fstream fd(path.c_str(), std::ios::in);
    char *buffer;
    buffer = (char *) malloc(BUFF_SIZE * sizeof(char));
    if (!fd.is_open()) {
      std::unique_lock lock(display_mutex);
      std::cout << "File " << file << " uploading failed (" << addr_str << ":" << port << ") couldn't open file" << std::endl;
      return false;
    }
    else {
      /* send whole file to server */
      while (!fd.eof()) {
        memset(buffer, 0, 1024);
        fd.read(buffer, 1024);
        if (send(tcp_sock.sock_no, (void *) buffer, fd.gcount(), 0) < 0) {
          std::unique_lock lock(display_mutex);
          std::cout << "File " << file << " uploading failed (" << addr_str << ":" << port << ") error in connect to server" << std::endl;
          return false;
        }
      }
      free(buffer);
      fd.close();
    }
  }
  close(tcp_sock.sock_no);
  return true;
}

void Client::send_remove(std::string data) {
  if (send(sock.sock_no, sock.local_addr, Simpl_cmd(global::cmd_message["DEL"], cmd_seq, data)) < 0)
    syserr("send");
}

void Client::exit() {
  group.clear();
  files.clear();
  files.shrink_to_fit();
  for (auto &thread: threads)
    if (thread.joinable())
      thread.join();
  threads.clear();
  threads.shrink_to_fit();
  close(sock.sock_no);
}

int main(int argc, char *argv[]) {
  struct client_param parameters {};
  parameters.timeout = TIMEOUT;

  /* parsing program's arguments */
  if (!client_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-client -g mcast_addr -p port -o path_to_dir [-t timeout]\n";
    fatal("arguments");
  }

  uint64_t cmd_seq = rand() % UINTMAX_MAX + 1;

  Client client(parameters, cmd_seq);
  client.listen();

  exit(EXIT_SUCCESS);
}