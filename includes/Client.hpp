#pragma once

#include <iostream>
#include <vector>
#include <errno.h>
#include <cstring>
class Server;

class Client
{
    private:
        int         _fd;
        std::string _ip;
        std::string _nick;
        std::string _user;
        std::string _realname;
        std::vector<std::string> _channels;
        bool       _full_connected;
        bool       _authenticated;
        Server     *_server;
        
    public:
        std::string _buffer;
        std::string _output_buffer;
        static const bool REMOVE_CLIENT_SOCKET = false;
        static const bool KEEP_CLIENT_SOCKET = true;
        Client();
        ~Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        void set_fd(int sockfd);
        void set_nick(const std::string &nick);
        void set_user(const std::string &user);
        void set_ip(const std::string &ip);
        void set_realname(const std::string &realname);
        void set_authenticated(bool auth);
        void set_full_connected(bool full);
        int get_fd() const;

        void set_server(Server *server);
        const std::string &get_nick() const;
        const std::string &get_user() const;
        const std::string &get_ip() const;
        const std::string &get_realname() const;
        bool is_authenticated() const;
        bool full_connected() const;

        bool execute(std::string &request);
        bool handle_CMD(std::vector<std::string> &req);
        void add_channel_to_client(const std::string &channel_name);
        void remove_channel_from_client(const std::string &channel_name);
        void add_to_queue(const std::string &message);
        void send(void);
        size_t get_channels_count() const;
        const std::vector<std::string> &get_channels_list() const;
};