#pragma once
#include "Client.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Server
{
private:
  static bool _signal;
  int _port;
  int _serv_socketfd;
  std::string _password;
  std::map<int, Client> _clients;
  std::vector<struct pollfd> _fds;

public:
  Server(int port, std::string password);
  void run();
  void acceptnewclient();
  void acceptclientcommands(int fd);
  static void signalhandler(int signum);

  std::string get_password() const;
  int get_port() const;
  bool isNickTaken(const std::string &nick, int exclude_fd) const;
  void removeClient(int fd);
};