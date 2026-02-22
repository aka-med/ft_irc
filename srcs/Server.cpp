#include "../includes/Server.hpp"

bool Server::_signal = false;

Server::Server(int port, std::string password)
{
    int                 op = 1;
    struct pollfd       server_poll;
    struct sockaddr_in  serv_addr;


    _password = password;
    _port = port;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(_port);

    _serv_socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(_serv_socketfd < 0)
        throw std::runtime_error("socket creation failed");

    if(setsockopt(_serv_socketfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op)) < 0)
        throw std::runtime_error("set SO_REUSEADDR option failed");

    if(fcntl(_serv_socketfd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("set O_NONBLOCK option failed");

    if(bind(_serv_socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("bind socket failed");

    if(listen(_serv_socketfd, SOMAXCONN) < 0)
        throw std::runtime_error("socket listen failed");

    server_poll.fd = _serv_socketfd;
    server_poll.events = POLLIN;
    server_poll.revents = 0;

    _fds.push_back(server_poll);
}


void Server::run()
{
    while(_signal == false)
    {
        if(poll(&_fds[0], _fds.size(), -1) < 0 && _signal == false)
            throw std::runtime_error("poll failed");
        for(size_t i = 0; i < _fds.size(); i++)
        {
            if(_fds[i].revents & POLLIN)
            {
                if(_fds[i].fd == _serv_socketfd)
                    acceptnewclient();//call the accept client function
                else
                    acceptclientcommands(_fds[i].fd);//call recieve command function
            }
        }
    }
}

void Server::acceptnewclient()
{
    Client              client;
    struct sockaddr_in  client_addr;
	struct pollfd       client_poll;
    int                 client_sockfd;
	socklen_t           len = sizeof(client_addr);

    
    client_sockfd = accept(_serv_socketfd, (sockaddr *)&client_addr, &len);
    if(client_sockfd < 0)
    {
        std::cout << "client accept failed" << std::endl;
        return;
    }
    if(fcntl(client_sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cout << "client fcntl failed" << std::endl;
        return;
    }

    client_poll.fd = client_sockfd;
	client_poll.events = POLLIN;
	client_poll.revents = 0;
	_fds.push_back(client_poll);

	client.set_clientsockfd(client_sockfd);
	_clients.push_back(client);

}

void Server::acceptclientcommands(int fd)
{
    //recieve data in chunks (tcp katsendi data bchenks)
    //parse the data
    //and then execute the data;
    (void)fd;
}

void Server::signalhandler(int signum)
{
    (void)signum;
    _signal = true;
}