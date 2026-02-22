#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client() : _fd(-1), _ip(""), _nick(""), _user(""), _authenticated(false), _buffer("")
{
    
}

void Client::set_fd(int sockfd)
{
    _fd = sockfd;
}

void Client::set_nick(std::string &nick)
{
    _nick = nick;
}

void Client::set_user(std::string &user)
{
    _user = user;
}

void Client::set_ip(std::string ip)
{
    _ip = ip;
}

int Client::get_fd() const
{
    return (_fd);
}


const std::string &Client::get_nick() const
{
    return (_nick);
}

const std::string &Client::get_user() const
{
    return (_user);
}

const std::string &Client::get_ip() const
{
    return (_ip);
}

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

void Client::handle_CMD(Server &server, std::vector<std::string> &cmds)
{
    if (cmds.empty())
        return;
    std::string cmd = cmds[0];
    if (cmd == "CAP")
    {
        if (cmds.size() > 1 && cmds[1] == "LS")
        {
            std::string reply = ":localhost CAP * LS :\r\n";
            send(_fd, reply.c_str(), reply.length(), 0);
        } 
    }
    else if (cmd == "PASS")
    {
        if (cmds.size() > 1)
        {
            std::string password = cmds[1];
            if (password == server.get_password())
            {
                _authenticated = true;
            }
            else
            {
                std::string reply = ":localhost 464 " + _nick + " :Password incorrect\r\n";
                send(_fd, reply.c_str(), reply.length(), 0);
            }
        }
    }
    else if (cmd == "NICK")
    {
        if (cmds.size() > 1 && !_authenticated)
        {
            set_nick(cmds[1]);
        }
        else
        {
            std::string reply = ":localhost 451 " + _nick + " :You must provide a password first\r\n";
            send(_fd, reply.c_str(), reply.length(), 0);
        }
    }
    else if (cmd == "USER")
    {
        if (cmds.size() > 4 && !_authenticated && !_nick.empty())
        {
            set_user(cmds[4]);
            std::string reply = ":localhost 001 " + _nick + " :Welcome to the IRC Server\r\n";
            send(_fd, reply.c_str(), reply.length(), 0);
        }
        else
        {
            std::string reply = ":localhost 451 " + _nick + " :You must provide a password and nickname first\r\n";
            send(_fd, reply.c_str(), reply.length(), 0);
        }
    }
}

void Client::execute(Server &server, std::string &req)
{
    std::cout << "Client " << _nick << " number " << _fd << " executed command: " << req << std::endl;
    std::vector<std::string> split_request = split(req);
    handle_CMD(server, split_request);
}
