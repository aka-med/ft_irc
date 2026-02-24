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
        std::string _realname;
        bool       _full_connected;
        bool       _authenticated;

    
    public:
        std::string _buffer;
        static const bool REMOVE_CLIENT_SOCKET = false;
        static const bool KEEP_CLIENT_SOCKET = true;
        Client();

        void set_fd(int sockfd);
        void set_nick(const std::string &nick);
        void set_user(const std::string &user);
        void set_ip(const std::string &ip);
        void set_realname(const std::string &realname);
        void set_authenticated(bool auth);
        void set_full_connected(bool full);

        int get_fd() const;
        const std::string &get_nick() const;
        const std::string &get_user() const;
        const std::string &get_ip() const;
        const std::string &get_realname() const;
        bool is_authenticated() const;
        bool full_conected() const;

        std::vector<std::string> split(const std::string &str);
        bool execute(Server &server, std::string &request);
        bool handle_CMD(Server &server, std::vector<std::string> &req);

        void send(const std::string &message) const;
};