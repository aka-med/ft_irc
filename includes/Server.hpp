#pragma once
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

class Channel;
class Client;
class Bot;

class Server
{
private:
  static bool _signal;
  int _port;
  int _serv_socketfd;
  std::string _password;
  std::string _server_name;
  std::map<int, Client *> _clients;
  std::vector<struct pollfd > _fds;
  std::map<std::string, Channel> _channels;
  Bot _bot;

public:
  static const bool FIRST_CHANNEL_JOIN = true;
  static const bool OTHER_CHANNEL_JOIN = false;
  Server(int port, std::string password, std::string server_name);
  ~Server();
  Server(const Server& other);
  Server& operator=(const Server& other);

  void run();
  void acceptnewclient();
  void acceptclientcommands(int fd);
  static void signalhandler(int signum);

  std::string get_password() const;
  int get_port() const;
  std::string get_name() const; 
  bool isNickTaken(const std::string &nick, int exclude_fd) const;
  void removeClient(int fd);

  void addClientToChannel(const std::string &channel_name, Client *client, bool first_join = false);
  void removeClientFromChannel(const std::string &channel_name, Client *client);

  bool channelExists(const std::string &channel_name) const;
  Channel *getChannel(const std::string &channel_name);
  Client *getClientByNick(const std::string &nick) const;
  void sendMessageToUser(const std::string &nick, const std::string &message);
  Bot &getBot();

  struct pollfd *getPollfdByClientFd(int client_fd);

};