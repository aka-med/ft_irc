#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Commands.hpp"

Client::Client()
    : _fd(-1), _ip(""), _nick(""), _user(""), _realname(""), _full_connected(false), _authenticated(false),
      _buffer("") {}

void Client::set_fd(int sockfd) { _fd = sockfd; }

void Client::set_nick(const std::string &nick) { _nick = nick; }

void Client::set_user(const std::string &user) { _user = user; }

void Client::set_realname(const std::string &realname) { _realname = realname; }

void Client::set_ip(const std::string &ip) { _ip = ip; }

void Client::set_authenticated(bool auth) { _authenticated = auth; }

void Client::set_full_connected(bool full) { _full_connected = full; }

int Client::get_fd() const { return (_fd); }

const std::string &Client::get_nick() const { return (_nick); }

const std::string &Client::get_user() const { return (_user); }

const std::string &Client::get_ip() const { return (_ip); }

const std::string &Client::get_realname() const { return (_realname); }

bool Client::is_authenticated() const { return (_authenticated); }

bool Client::full_conected() const { return (_full_connected); }

std::vector<std::string> Client::split(const std::string &req)
{
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

void Client::send(const std::string &message) const
{
    ::send(_fd, message.c_str(), message.size(), 0);
}

bool Client::handle_CMD(Server &server, std::vector<std::string> &cmds)
{
    if(cmds.size() < 2)
        return Client::KEEP_CLIENT_SOCKET;

    if (Utils::to_upper(cmds[0]) == "CAP")
        return Command::cap(server, *this, cmds);
    if(Utils::to_upper(cmds[0]) == "PASS")
        return Command::pass(server, *this, cmds.size() > 1 ? cmds[1] : "");
    else if (Utils::to_upper(cmds[0]) == "NICK")
        return Command::nick(server, *this, cmds.size() > 1 ? cmds[1] : "");
    else if(Utils::to_upper(cmds[0]) == "USER")
        return Command::user(server, *this, cmds);
    else if(Utils::to_upper(cmds[0]) == "MODE")
        return Command::mode(server, *this, cmds);
    else if(Utils::to_upper(cmds[0]) == "PING")
        return Command::ping(server, *this, cmds);
    else if(Utils::to_upper(cmds[0]) == "JOIN")
        return Command::join(server, *this, cmds);
    else if(Utils::to_upper(cmds[0]) == "PRIVMSG")
        return Command::privmsg(server, *this, cmds);
    return Client::KEEP_CLIENT_SOCKET;
}

bool Client::execute(Server &server, std::string &req)
{
  std::cout << "Client " << _nick << " number " << _fd
            << " executed command: " << req << std::endl;
  std::vector<std::string> split_request = split(req);
  return handle_CMD(server, split_request);
}
