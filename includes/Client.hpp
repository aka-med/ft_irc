#pragma once

#include <iostream>
#include <vector>
class Server;

class Client
{
    private:
        int         _fd;
        std::string _ip;
        std::string _nick;
        std::string _user;
        bool       _authenticated;

    
    public:
        std::string _buffer;   
        Client();

        void set_fd(int sockfd);
        void set_nick(std::string &nick);
        void set_user(std::string &user);
        void set_ip(std::string ip);

        int get_fd() const;
        const std::string &get_nick() const;
        const std::string &get_user() const;
        const std::string &get_ip() const;

        std::vector<std::string> split(const std::string &str);
        void execute(Server &server, std::string &request);
        void handle_CMD(Server &server, std::vector<std::string> &req);
};