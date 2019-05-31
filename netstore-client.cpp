#include "netstore-client.h"
#include "err.h"

Client::Client(struct client_param parameters, uint64_t _seq): parameters(parameters), cmd_seq(_seq), sock {SOCK_DGRAM} {}

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

void Client::send_discover() {
  sockaddr_in rec_addr {};
  Cmplx_cmd cmplx_cmd {};
  memset(&rec_addr, 0, sizeof rec_addr);
  timeval recv_timeout {};

  if (send(sock.sock_no, sock.local_addr, Simpl_cmd(global::cmd_message["HELLO"], cmd_seq, global::empty_str)) < 0)
    syserr("send");

  auto time = std::chrono::system_clock::now();
  while (get_diff(time) < parameters.timeout) {
    set_recvtime(&recv_timeout, time);
    sock.set_timeout(recv_timeout);
    if (receive(sock.sock_no, rec_addr, cmplx_cmd) > 0) {
      std::cout << "Found " << inet_ntoa(rec_addr.sin_addr) << " (" << cmplx_cmd.data << ") with free space "\
      << be64toh(cmplx_cmd.param) << std::endl;
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
  while (get_diff(time) < parameters.timeout) {
    set_recvtime(&recv_timeout, time);
    sock.set_timeout(recv_timeout);
    if (receive(sock.sock_no, rec_addr, simpl_cmd) > 0)
      found_files(inet_ntoa(rec_addr.sin_addr), simpl_cmd.data);
  }
  print_files();
}

void Client::found_files(char * addr, char * data) {
  std::stringstream stream(data);
  std::string addr_str(addr);
  std::string file;
  while (getline(stream, file, '\n'))
    files.emplace_back(std::make_pair(file, addr_str));
}

void Client::print_files() {
  for (auto &pair: files)
    std::cout << pair.first << " (" << pair.second << ")" << std::endl;
}

void Client::send_fetch(std::string data) {
  std::string addr_str;
  for (auto &pair: files) {
    if (pair.first == data) {
      addr_str = pair.second;
      break;
    }
  }

  if (addr_str != global::empty_str) {
    Sock fetch_sock {SOCK_DGRAM};
    sockaddr_in rec_addr{};
    Cmplx_cmd cmplx_cmd{};
    fetch_sock.set_address(addr_str.data(), parameters.cmd_port);
    timeval timeout{};
    timeout.tv_sec = parameters.timeout / 1000;
    fetch_sock.set_timeout(timeout);
    if (send(fetch_sock.sock_no, fetch_sock.local_addr, Simpl_cmd(global::cmd_message["GET"], cmd_seq, data)) < 0)
      syserr("send");
    if (receive(fetch_sock.sock_no, rec_addr, cmplx_cmd) > 0) {
      std::cout << "answer from server: " << be64toh(cmplx_cmd.param) << std::endl;
      signed short port = be64toh(cmplx_cmd.param);
      std::string port_str(std::to_string(port));
      connect_to_tcp(addr_str, port_str);

//      std::thread thread(download_file, rec_addr.sin_addr.s_addr, std::ref(port));
//      thread.join();
//      std::cout << "ended" << std::endl;
    }
  } else
    std::cout << "file doesn't exist" << std::endl;
}

void Client::connect_to_tcp(std::string &addr, std::string &port) {
  int err;
  addrinfo addr_hints{};
  addrinfo *addr_result;

  memset(&addr_hints, 0, sizeof(struct addrinfo));
  addr_hints.ai_family = AF_INET;
  addr_hints.ai_socktype = SOCK_STREAM;
  addr_hints.ai_protocol = IPPROTO_TCP;
  err = getaddrinfo(addr.c_str(), port.c_str(), &addr_hints, &addr_result);

  if (err == EAI_SYSTEM)
    syserr("getaddrinfo: %s", gai_strerror(err));
  else if (err != 0)
    fatal("getaddrinfo: %s", gai_strerror(err));

  Sock tcp_sock{addr_result->ai_socktype};
  tcp_sock.set_address(addr.c_str(), )

  if (::connect(tcp_sock.sock_no, addr_result->ai_addr, addr_result->ai_addrlen) < 0)
    syserr("connect");

  freeaddrinfo(addr_result);

}

void Client::download_file(std::string addr, signed short &port) {
  Sock tcp_sock{SOCK_STREAM};
  std::cout << addr << " " << port << std::endl;
  tcp_sock.set_address(addr.data(), port);
  if (::connect(tcp_sock.sock_no, (sockaddr *)&(tcp_sock.local_addr), sizeof(tcp_sock.local_addr)) < 0)
    syserr("connect");
}

void Client::send_upload(std::string data) {
  if (data[0] != '/') {

  }
}

void Client::send_remove(std::string data) {

}

int main(int argc, char *argv[]) {
  struct client_param parameters {};
  parameters.timeout = TIMEOUT;
  std::vector<std::string> line;
  bool exit = false;

  /* parsing program's arguments */
  if (!client_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-client -g mcast_addr -p port -o path_to_dir [-t timeout]\n";
    fatal("arguments");
  }

  Client client(parameters, 2);
  client.connect();

  while (!exit) {
    line = client.get_command();
    if (line.size() == 2) {
      if (line[0] == "search")
        client.send_search(line[1]);
      if (line[0] == "fetch")
        client.send_fetch(line[1]);
      if (line[0] == "upload")
        client.send_upload(line[1]);
      if (line[0] == "remove")
        client.send_remove(line[1]);
    } else if (line.size() == 1) {
      if (line[0] == "discover")
        client.send_discover();
      if (line[0] == "search")
        client.send_search(global::empty_str);
      if (line[0] == "exit") //TODO close all sockets
        exit = true;
    }
  }

}