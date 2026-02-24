#include "../includes/Server.hpp"
#include <arpa/inet.h>

bool Server::_signal = false;

Server::Server(int port, std::string password, std::string server_name) : _server_name(server_name)
{
  int op = 1;
  struct pollfd server_poll;
  struct sockaddr_in serv_addr;

  _password = password;
  _port = port;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(_port);

  _serv_socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (_serv_socketfd < 0)
    throw std::runtime_error("socket creation failed");

  if (setsockopt(_serv_socketfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op)) < 0)
    throw std::runtime_error("set SO_REUSEADDR option failed");

  if (fcntl(_serv_socketfd, F_SETFL, O_NONBLOCK) < 0)
    throw std::runtime_error("set O_NONBLOCK option failed");

  if (bind(_serv_socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0)
    throw std::runtime_error("bind socket failed");

  if (listen(_serv_socketfd, SOMAXCONN) < 0)
    throw std::runtime_error("socket listen failed");

  server_poll.fd = _serv_socketfd;
  server_poll.events = POLLIN;
  server_poll.revents = 0;

  _fds.push_back(server_poll);
}

void Server::run() {
  while (_signal == false) {
    if (poll(&_fds[0], _fds.size(), -1) < 0 && _signal == false)
      throw std::runtime_error("poll failed");
    for (size_t i = 0; i < _fds.size(); i++) {
      if (_fds[i].revents & POLLIN) {
        if (_fds[i].fd == _serv_socketfd)
          acceptnewclient(); // call the accept client function
        else
                    acceptclientcommands(_fds[i].fd);//call recieve command function
      }
    }
  }
}

void Server::acceptnewclient()
{
    Client              client;
    struct sockaddr_in  client_addr;
	struct pollfd       client_poll;
    int                 client_sockfd;
	socklen_t           len = sizeof(client_addr);


  client_sockfd = accept(_serv_socketfd, (sockaddr *)&client_addr, &len);
    if(client_sockfd < 0)
    {
    std::cout << "client accept failed" << std::endl;
    return;
  }
    if(fcntl(client_sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
    std::cout << "client fcntl failed" << std::endl;
    return;
  }

  client_poll.fd = client_sockfd;
  client_poll.events = POLLIN;
  client_poll.revents = 0;
  client.set_ip(inet_ntoa(client_addr.sin_addr));
  client.set_fd(client_sockfd);
  _clients[client_sockfd] = client;
  _fds.push_back(client_poll);
}

  void Server::acceptclientcommands(int fd)
  {
    char buffer[1024];
    int bytes;
    size_t pos;
    std::string &buf = _clients[fd]._buffer;

    bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)
    {
      removeClient(fd);
      return;
    }
    buffer[bytes] = '\0';
    _clients[fd]._buffer += buffer;
    while ((pos = buf.find("\r\n")) != std::string::npos)
    {
      std::string request = buf.substr(0, pos);
      buf.erase(0, pos + 2);
      if (!request.empty())
      {
        if (!_clients[fd].execute(*this, request))
        {
          removeClient(fd);
          return;
        }
      }
    }
  }

void Server::signalhandler(int signum)
{
  (void)signum;
  _signal = true;
}

int Server::get_port() const
{
    return _port;
}

std::string Server::get_name() const
{
    return _server_name;
}

std::string Server::get_password() const { return _password; }

bool Server::isNickTaken(const std::string &nick, int exclude_fd) const
{
  std::map<int, Client>::const_iterator it;

  for (it = _clients.begin(); it != _clients.end(); ++it)
    if (it->first != exclude_fd && it->second.get_nick() == nick)
      return true;
  
  return false;
}

bool Server::channelExists(const std::string &channel_name) const
{
    return _channels.find(channel_name) != _channels.end();
}

void Server::removeClient(int fd)
{
  close(fd);
  _clients.erase(fd);
  for (size_t i = 0; i < _fds.size(); i++)
  {
    if (_fds[i].fd == fd)
    {
      _fds.erase(_fds.begin() + i);
      break;
    }
  }
}

void Server::addClientToChannel(const std::string &channel_name, const Client &client, bool first_join)
{
    if (first_join)
    {
        Channel new_channel(channel_name);
        new_channel.add_user(client);
        new_channel.add_operator(client);
        _channels[channel_name].push_back(new_channel);
        return;
    }
    std::vector<Channel> &channels = _channels[channel_name];
    for(size_t i = 0; i < channels.size(); i++)
    {
        if(channels[i].get_name() == channel_name)
        {
            channels[i].add_user(client);
            break; 
        }
    }
}

void Server::removeClientFromChannel(const std::string &channel_name, const Client &client)
{
  std::vector<Channel> &channels = _channels[channel_name];
  for(size_t i = 0; i < channels.size(); i++)
  {
    if(channels[i].get_name() == channel_name)
    {
      channels[i].remove_user(client);
      break;
    }
  }
}

Channel *Server::getChannel(const std::string &channel_name)
{
    std::vector<Channel> &channels = _channels[channel_name];
    for(size_t i = 0; i < channels.size(); i++)
    {
        if(channels[i].get_name() == channel_name)
        {
            return &channels[i];
        }
    }
    return NULL;
}

Client *Server::getClientByNick(const std::string &nick) const
{
    std::map<int, Client>::const_iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if (it->second.get_nick() == nick)
            return const_cast<Client *>(&(it->second));
    
    return NULL;
}

void Server::sendMessageToUser(const std::string &nick, const std::string &message)
{
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.get_nick() == nick)
        {
            it->second.send(message);
            break;
        }
    }
}