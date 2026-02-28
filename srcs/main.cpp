#include "../includes/Bot.hpp"
#include "../includes/Server.hpp"
#include "../includes/Parser.hpp"

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }

    int port = Parser::validatePort(av[1]);
    if (port == -1)
    {
        std::cerr << "Error: Invalid port. Must be between 1024 and 65535." << std::endl;
        return (1);
    }

    if (!Parser::validatePassword(av[2]))
    {
        std::cerr << "Error: Invalid password. Must be 1-128 characters and contain no spaces/newlines." << std::endl;
        return (1);
    }

    try
    {
        
        signal(SIGPIPE, SIG_IGN);
		signal(SIGQUIT, Server::signalhandler);
        signal(SIGINT, Server::signalhandler);
        Server server(port, av[2], "ft_irc_server");
        server.run();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    
    return (0);
}