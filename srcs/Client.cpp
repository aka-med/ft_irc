#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client()
    : _fd(-1), _ip(""), _nick(""), _user(""), _authenticated(false),
      _buffer("") {}

void Client::set_fd(int sockfd) { _fd = sockfd; }

void Client::set_nick(std::string &nick) { _nick = nick; }

void Client::set_user(std::string &user) { _user = user; }

void Client::set_ip(std::string ip) { _ip = ip; }

int Client::get_fd() const { return (_fd); }

const std::string &Client::get_nick() const { return (_nick); }

const std::string &Client::get_user() const { return (_user); }

const std::string &Client::get_ip() const { return (_ip); }

std::vector<std::string> Client::split(const std::string &req) {
  std::vector<std::string> tokens;
  size_t colon_pos = req.find(':');
  std::string pre_colon = req.substr(0, colon_pos);

  std::istringstream iss(pre_colon);
  std::string s;

  while (iss >> s)
    tokens.push_back(s);

  if (colon_pos != std::string::npos)
    tokens.push_back(req.substr(colon_pos + 1));
  return tokens;
}

void Client::handle_CMD(Server &server, std::vector<std::string> &cmds) {
  if (cmds.empty())
    return;
  std::string cmd = cmds[0];
  std::string reply;

  if (cmd == "CAP") {
    if (cmds.size() > 1 && cmds[1] == "LS") {
      reply = ":localhost CAP * LS :\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
    }
  } else if (cmd == "PASS") {
    if (cmds.size() > 1) {
      std::string password = cmds[1];
      if (password == server.get_password()) {
        _authenticated = true;
      } else {
        std::string reply =
            ":localhost 464 " + _nick + " :Password incorrect\r\n";
        send(_fd, reply.c_str(), reply.length(), 0);
      }
    }
  } else if (cmd == "NICK") {
    if (!_authenticated) {
      reply =
          ":localhost 451 " + _nick + " :You must provide a password first\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
      return;
    }
    if (cmds.size() < 2) {
      reply = ":localhost 431 " + _nick + " :No nickname given\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
      return;
    }
    std::string old_nick = _nick;
    set_nick(cmds[1]);
    if (!old_nick.empty()) {
      reply = ":" + old_nick + " NICK " + _nick + "\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
    }
  } else if (cmd == "USER") {
    if (!_authenticated || _nick.empty()) {
      reply = ":localhost 451 " + _nick +
              " :You must provide a password and nickname first\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
      return;
    }
    if (cmds.size() > 1) {
      set_user(cmds[1]);
      reply = ":localhost 001 " + _nick + " :Welcome to the IRC Server " +
              _nick + "\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
      reply = ":localhost 002 " + _nick +
              " :Your host is localhost, running version 1.0\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
      reply = ":localhost 003 " + _nick + " :This server was created today\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
      reply = ":localhost 004 " + _nick + " localhost 1.0 o o\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
    }
  } else if (cmd == "PING") {
    std::string token = (cmds.size() > 1) ? cmds[1] : "localhost";
    reply = ":localhost PONG localhost :" + token + "\r\n";
    send(_fd, reply.c_str(), reply.length(), 0);
  } else if (cmd == "MODE") {
    if (cmds.size() > 1) {
      reply = ":localhost 221 " + _nick + " +\r\n";
      send(_fd, reply.c_str(), reply.length(), 0);
    }
  } else if (cmd == "QUIT") {
    reply = ":localhost ERROR :Closing connection\r\n";
    send(_fd, reply.c_str(), reply.length(), 0);
    server.removeClient(_fd);
  }
}

void Client::execute(Server &server, std::string &req) {
  std::cout << "Client " << _nick << " number " << _fd
            << " executed command: " << req << std::endl;
  std::vector<std::string> split_request = split(req);
  handle_CMD(server, split_request);
}
