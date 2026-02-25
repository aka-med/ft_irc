#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>
class Server;
class Client;

namespace Command
{
    bool pass(Server &server, Client &client, const std::string &password);
    bool cap(Server &server, Client &client, const std::vector<std::string> &args);
    bool nick(Server &server, Client &client, const std::string &nickname);
    bool user(Server &server, Client &client, const std::vector<std::string> &args);
    bool mode(Server &server, Client &client, const std::vector<std::string> &args);
    bool ping(Server &server, Client &client, const std::vector<std::string> &args);
    bool join(Server &server, Client &client, const std::vector<std::string> &args);
    bool privmsg(Server &server, Client &client, const std::vector<std::string> &args);
    bool part(Server &server, Client &client, const std::vector<std::string> &args);
    bool quit(Server &server, Client &client, const std::vector<std::string> &args);
    bool kick(Server &server, Client &client, const std::vector<std::string> &args);
    bool unknown(Server &server, Client &client, const std::vector<std::string> &args);
};
#endif 