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
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <map>
#include <sstream>

class Server
{
    private:
        static bool                 _signal;                   
        int                         _port;
        int                         _serv_socketfd;
        std::string                 _password;
        std::map<int, Client>        _clients;
        std::vector<struct pollfd>  _fds;

    public:
        Server(int port, std::string password);
        void run();
        void acceptnewclient();
        void acceptclientcommands(int fd);
        static void signalhandler(int signum);

        std::string get_password() const;
        int get_port() const;


};