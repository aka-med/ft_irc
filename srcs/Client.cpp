#include "../includes/Client.hpp"

Client::Client()
{
    
}

void Client::set_clientsockfd(int sockfd)
{
    _client_sockfd = sockfd;
}


void Client::set_nick(std::string &nick)
{
    _nick = nick;
}

void Client::set_user(std::string &user)
{
    _user = user;
}


int Client::get_clientsockfd() const
{
    return (_client_sockfd);
}


const std::string &Client::get_nick() const
{
    return (_nick);
}
const std::string &Client::get_user() const
{
    return (_user);
}