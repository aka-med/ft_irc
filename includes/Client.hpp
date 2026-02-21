#pragma once

#include <iostream>

class Client
{
    private:
        int         _client_sockfd;
        std::string _nick;
        std::string _user;        
    public:
        Client();

        void set_clientsockfd(int sockfd);
        void set_nick(std::string &nick);
        void set_user(std::string &user);


        int get_clientsockfd() const;
        const std::string &get_nick() const;
        const std::string &get_user() const;
};