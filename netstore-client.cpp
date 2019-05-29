#include <chrono>
#include "netstore-client.h"
#include "err.h"

Client::Client(struct client_param parameters, uint64_t _seq): parameters(parameters), cmd_seq(_seq), sock {} {}

void Client::connect() {
  sock.enable_broadcasting();
  sock.set_address(parameters.mcast_addr, parameters.cmd_port);
}

template <typename clock>
unsigned int Client::get_diff(std::chrono::time_point<clock> time) {
  auto time_now = std::chrono::system_clock::now();
  auto milisecs = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time).count();
  return milisecs;
}

template <typename clock>
void Client::set_recvtime(timeval *recv_timeout, std::chrono::time_point<clock> time) {
  auto time_now = std::chrono::system_clock::now();
  auto milisecs = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time).count();
  recv_timeout->tv_sec = milisecs / 1000;
  recv_timeout->tv_usec = (milisecs % 1000) * 1000;
}

void Client::send_hello() {
  sockaddr_in rec_addr {};
  Cmplx_cmd cmplx_cmd {};
  memset(&rec_addr, 0, sizeof rec_addr);
  timeval recv_timeout {};

  if (send(sock.sock_no, sock.local_addr, Simpl_cmd(cmd_message[0], cmd_seq, empty_str)) < 0)
    syserr("send");

  auto time = std::chrono::system_clock::now();
  while (get_diff(time) < parameters.timeout) {
    set_recvtime(&recv_timeout, time);
    sock.set_timeout(recv_timeout);
    if (receive(sock.sock_no, rec_addr, cmplx_cmd) > 0) {
      std::cout << "Found " << inet_ntoa(rec_addr.sin_addr) << " with message: " << cmplx_cmd.cmd \
      << " with max_space: " << be64toh(cmplx_cmd.param) << ", mcast_addr: " << cmplx_cmd.data << "\n";
    }
  }
}

void Client::send_list(const char *const data) {
  sockaddr_in rec_addr {};
  Simpl_cmd simpl_cmd {};
  memset(&rec_addr, 0, sizeof rec_addr);

  if (send(sock.sock_no, sock.local_addr, Simpl_cmd(cmd_message[2], cmd_seq, data)) < 0)
    syserr("send");


  if (receive(sock.sock_no, rec_addr, simpl_cmd) > 0) {
    std::cout << "Found " << inet_ntoa(rec_addr.sin_addr) << " with message: " << simpl_cmd.cmd \
      << " with list of files: " << simpl_cmd.data << "\n";
  }
}

int main(int argc, char *argv[]) {
  struct client_param parameters;
  parameters.timeout = TIMEOUT;

  /* parsing program's arguments */
  if (!client_parse(argc, argv, &parameters)) {
    std::cout << "Usage: ./netstore-client -g mcast_addr -p port -o path_to_dir [-t timeout]\n";
    fatal("arguments");
  }

  Client client(parameters, 2);
  client.connect();
  client.send_hello();
  const char *const str = {"err"};
  client.send_list(str);
}