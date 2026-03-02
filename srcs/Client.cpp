#include "../includes/Client.hpp"
#include "../includes/Bot.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Commands.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Parser.hpp"

Client::Client()
    : _fd(-1), _ip(""), _nick(""), _user(""), _realname(""), _full_connected(false), _authenticated(false),
    _server(NULL), _buffer(""), _output_buffer("") {}

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

bool Client::full_connected() const { return (_full_connected); }

void Client::set_server(Server *server) { _server = server; }

void Client::send(void)
{
    if (_fd == -1 || _output_buffer.empty())
        return;
    ssize_t bytes_sent = ::send(_fd, _output_buffer.c_str(), _output_buffer.size(), 0);
    if (bytes_sent < 0)
    {
        std::cerr << "Failed to send message to client " << _nick << ": " << strerror(errno) << std::endl;
        return;
    }
    _output_buffer.erase(0, bytes_sent);
    if (_output_buffer.empty())
    {  
        struct pollfd *pollfd = _server->getPollfdByClientFd(_fd);
        if (pollfd)
            pollfd->events &= ~POLLOUT;
    }
}

bool Client::handle_CMD(std::vector<std::string> &cmds)
{
    if(cmds.size() < 1)
        return Client::KEEP_CLIENT_SOCKET;

    if (Utils::to_upper(cmds[0]) == "CAP")
        return Command::cap(_server, this, cmds);
    if(Utils::to_upper(cmds[0]) == "PASS")
        return Command::pass(_server, this, cmds.size() > 1 ? cmds[1] : "");
    else if (Utils::to_upper(cmds[0]) == "NICK")
        return Command::nick(_server, this, cmds.size() > 1 ? cmds[1] : "");
    else if(Utils::to_upper(cmds[0]) == "USER")
        return Command::user(_server, this, cmds);
    else if (Utils::to_upper(cmds[0]) == "QUIT")
            return Command::quit(_server, this, cmds);
    else if(full_connected())
    {
        if(Utils::to_upper(cmds[0]) == "MODE")
            return Command::mode(_server, this, cmds);
        else if(Utils::to_upper(cmds[0]) == "PING")
            return Command::ping(_server, this, cmds);
        else if(Utils::to_upper(cmds[0]) == "JOIN")
            return Command::join(_server, this, cmds);
        else if(Utils::to_upper(cmds[0]) == "PRIVMSG")
            return Command::privmsg(_server, this, cmds);
        else if (Utils::to_upper(cmds[0]) == "PART")
            return Command::part(_server, this, cmds);
        else if (Utils::to_upper(cmds[0]) == "KICK")
            return Command::kick(_server, this, cmds);
        else if (Utils::to_upper(cmds[0]) == "INVITE")
            return Command::invite(_server, this, cmds);
        else if (Utils::to_upper(cmds[0]) == "TOPIC")
            return Command::topic(_server, this, cmds);
        else
            return Command::unknown(_server, this, cmds);
    }
    else
    {
        std::string reply = Reply::format_reply(_server->get_name(), Reply::ERR_NOTREGISTERED, get_nick(), "", "You have not registered");
        add_to_queue(reply);
        return (KEEP_CLIENT_SOCKET);
    }
}

bool Client::execute(std::string &req)
{
  std::cout << "Client " << _nick << " number " << _fd
            << " executed command: " << req << std::endl;
  
  IRCMessage msg = Parser::parseLine(req);
  if (msg.empty())
    return KEEP_CLIENT_SOCKET;

  std::vector<std::string> split_request;
  split_request.push_back(msg.command);
  for (size_t i = 0; i < msg.params.size(); ++i)
    split_request.push_back(msg.params[i]);

  return handle_CMD(split_request);
}

void Client::add_channel_to_client(const std::string &channel_name)
{
    _channels.push_back(channel_name);
}

void Client::remove_channel_from_client(const std::string &channel_name)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i] == channel_name)
        {
            _channels.erase(_channels.begin() + i);
            break;
        }
    }
}

size_t Client::get_channels_count() const
{
    return _channels.size();
}

const std::vector<std::string> &Client::get_channels_list() const
{
    return _channels;
}

Client::~Client()
{
  if (_fd != -1)
  {
    close(_fd);
    _fd = -1;
  }
}

Client::Client(const Client& other)
    : _fd(other._fd), _ip(other._ip), _nick(other._nick), _user(other._user), _realname(other._realname),
      _channels(other._channels), _full_connected(other._full_connected), _authenticated(other._authenticated),
      _server(other._server), _buffer(other._buffer), _output_buffer(other._output_buffer) {}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        _fd = other._fd;
        _ip = other._ip;
        _nick = other._nick;
        _user = other._user;
        _realname = other._realname;
        _channels = other._channels;
        _full_connected = other._full_connected;
        _authenticated = other._authenticated;
        _buffer = other._buffer;
        _output_buffer = other._output_buffer;
        _server = other._server;
    }
    return *this;
}

void Client::add_to_queue(const std::string &message)
{
    if (!_server || message.empty())
        return;
    _output_buffer += message;

    if (!_output_buffer.empty())
    {
        struct pollfd *pollfd = _server->getPollfdByClientFd(_fd);
        if (pollfd)
            pollfd->events |= POLLOUT;
    }
}