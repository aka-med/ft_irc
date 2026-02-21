#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdexcept>
#include <fcntl.h>
#include <netinet/in.h>
#include <cstdlib>
#include <vector>
#include <poll.h>
#include <arpa/inet.h>
#include "Client.hpp"

class Server
{
    private:
        int                         _port;
        int                         _serv_socketfd;
        std::string                 _password;
        std::vector<Client>         _clients;
        std::vector<struct pollfd>  _fds;

    public:
        Server(int port, std::string password);
        void run();
        void acceptnewclient();
        void acceptclientcommands(int fd);


};