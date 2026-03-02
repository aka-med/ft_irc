#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>
#include <limits>
#include <string>
class Server;
class Client;
class Channel;

namespace Command
{
    bool pass(Server *server, Client *client, const std::string &password);
    bool cap(Server *server, Client *client, const std::vector<std::string> &args);
    bool nick(Server *server, Client *client, const std::string &nickname);
    bool user(Server *server, Client *client, const std::vector<std::string> &args);

    bool mode(Server *server, Client *client, const std::vector<std::string> &args);
    bool topic(Server *server, Client *client, const std::vector<std::string> &args);
    bool invite(Server *server, Client *client, const std::vector<std::string> &args);

    bool ping(Server *server, Client *client, const std::vector<std::string> &args);
    bool join(Server *server, Client *client, const std::vector<std::string> &args);
    bool privmsg(Server *server, Client *client, const std::vector<std::string> &args);
    bool part(Server *server, Client *client, const std::vector<std::string> &args);
    bool quit(Server *server, Client *client, const std::vector<std::string> &args);
    bool kick(Server *server, Client *client, const std::vector<std::string> &args);
    bool unknown(Server *server, Client *client, const std::vector<std::string> &args);
};


typedef struct mode_data_s
{
    Server *server;
    Client *client;
    Channel &channel;
    const std::vector<std::string> &args;
    std::string track_flags;
    std::string track_params;
    size_t params_index;
    char sign;
    char prev_sign;

    mode_data_s(Server *server, Client *client, Channel &channel, const std::vector<std::string> &args);

} mode_data_t;

#endif