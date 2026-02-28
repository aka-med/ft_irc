#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Bot.hpp"
#include "../includes/Server.hpp"
#include <arpa/inet.h>

bool Server::_signal = false;

Server::Server(int port, std::string password, std::string server_name) : _server_name(server_name), _bot("CalcBot")
{
  int op = 1;
  struct pollfd server_poll;
  struct sockaddr_in serv_addr;

  _password = password;
  _port = port;
  _bot.start();

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(_port);

  _serv_socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (_serv_socketfd < 0)
    throw std::runtime_error("socket creation failed");

  if (setsockopt(_serv_socketfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op)) < 0)
  {
    close(_serv_socketfd);
    throw std::runtime_error("set SO_REUSEADDR option failed");
  }

  if (fcntl(_serv_socketfd, F_SETFL, O_NONBLOCK) < 0)
  {
    close(_serv_socketfd);
    throw std::runtime_error("set O_NONBLOCK option failed");
  }

  if (bind(_serv_socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0)
  {
    close(_serv_socketfd);
    throw std::runtime_error("bind socket failed");
  }

  if (listen(_serv_socketfd, SOMAXCONN) < 0)
  {
    close(_serv_socketfd);
    throw std::runtime_error("socket listen failed");
  }

  server_poll.fd = _serv_socketfd;
  server_poll.events = POLLIN;
  server_poll.revents = 0;

  _fds.push_back(server_poll);
}

void Server::run()
{
  while (_signal == false)
  {
    if (poll(&_fds[0], _fds.size(), -1) == -1 && _signal == false)
      throw std::runtime_error("poll failed");
    for (size_t i = 0; i < _fds.size(); i++)
    {
      if (_fds[i].revents & POLLIN)
      {
        if (_fds[i].fd == _serv_socketfd)
          acceptnewclient(); 
        else
          acceptclientcommands(_fds[i].fd);
      }
      else if (_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
      {
        removeClient(_fds[i].fd);
        i--;
      }
    }
  }
}

void Server::acceptnewclient()
{
  Client              *client = new Client();
  struct sockaddr_in  client_addr;
	struct pollfd       client_poll;
  int                 client_sockfd;
	socklen_t           len = sizeof(client_addr);


  client_sockfd = accept(_serv_socketfd, (sockaddr *)&client_addr, &len);
  if(client_sockfd < 0)
  {
    delete client;
    std::cout << "client accept failed" << std::endl;
    return;
  }
  if(fcntl(client_sockfd, F_SETFL, O_NONBLOCK) < 0)
  {
    delete client;
    close(client_sockfd);
    std::cout << "client fcntl failed" << std::endl;
    return;
  }

  client_poll.fd = client_sockfd;
  client_poll.events = POLLIN;
  client_poll.revents = 0;
  client->set_ip(inet_ntoa(client_addr.sin_addr));
  client->set_fd(client_sockfd);
  _clients[client_sockfd] = client;
  _fds.push_back(client_poll);
}

  void Server::acceptclientcommands(int fd)
  {
    char buffer[1024];
    int bytes;
    size_t pos;
    std::string &buf = _clients[fd]->_buffer;

    bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)
    {
      removeClient(fd);
      return;
    }
    buffer[bytes] = '\0';
    buf += buffer;
    while ((pos = buf.find("\n")) != std::string::npos)
    {
      std::string request = buf.substr(0, pos);
      buf.erase(0, pos + 1);
      
      if (!request.empty() && request[request.size() - 1] == '\r')
          request.erase(request.size() - 1);
          
      if (!request.empty())
      {
        if (!_clients[fd]->execute(*this, request))
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
    for (std::map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->get_nick() == nick && it->second->get_fd() != exclude_fd)
            return true;
    }
    return false;
}

bool Server::channelExists(const std::string &channel_name) const
{
    return _channels.find(channel_name) != _channels.end();
}

void Server::removeClient(int fd)
{
  std::map<std::string, Channel>::iterator it = _channels.begin();
  while (it != _channels.end())
  {
    std::string channel_name = it->first;
    it++;
    removeClientFromChannel(channel_name, _clients[fd]);
  }
  delete _clients[fd];
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

void Server::addClientToChannel(const std::string &channel_name, Client *client, bool first_join)
{
  if (!client)
    return;
  if (first_join)
  {
      Channel new_channel(channel_name);
      new_channel.add_user(client);
      new_channel.add_operator(client);
      _channels[channel_name] = new_channel;
      return;
  }
  _channels[channel_name].add_user(client);
}

void Server::removeClientFromChannel(const std::string &channel_name, Client *client)
{
  std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
  if (it == _channels.end())
    return;
  Channel &channel = it->second;
  channel.remove_user(client);
  if (channel.is_operator(client))
    channel.remove_operator(client);
  if (channel.is_user_invited(client))
    channel.remove_invited_user(client);
  if (channel.get_users_count() == 0)
    _channels.erase(it);
}

Channel *Server::getChannel(const std::string &channel_name)
{
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it != _channels.end())
        return &(it->second);
    return NULL;
}

Client *Server::getClientByNick(const std::string &nick) const
{
    for (std::map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->get_nick() == nick)
            return it->second;
    }
    return NULL;
}

void Server::sendMessageToUser(const std::string &nick, const std::string &message)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->get_nick() == nick)
        {
            it->second->send(message);
            break;
        }
    }
}

Bot &Server::getBot()
{
    return _bot;
}

Server::~Server()
{
  if (_serv_socketfd != -1)
  {
    close(_serv_socketfd);
    _serv_socketfd = -1;
  }
  for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    delete it->second;
  _channels.clear();
  _fds.clear();
  _clients.clear();
  _bot.stop();
}

Server &Server::operator=(const Server& other)
{
    if (this != &other)
    {
        _port = other._port;
        _password = other._password;
        _server_name = other._server_name;
        for(std::map<int, Client *>::const_iterator it = other._clients.begin(); it != other._clients.end(); ++it)
            _clients[it->first] = new Client(*(it->second));
        _fds = other._fds;
        _channels = other._channels;
        _bot = other._bot;
    }
    return *this;
}

Server::Server(const Server& other)
{
  *this = other;
}