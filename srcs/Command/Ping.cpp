#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Utils.hpp"

bool Command::ping(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client.get_nick().empty() ? "*" : client.get_nick();
    if (!client.is_authenticated())
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOTREGISTERED, target, "", "You have not registered");
        client.send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (args.size() < 2)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "PING", "Not enough parameters");
        client.send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    reply = Reply::format_reply(server.get_name(), "PONG", target, "", args[1]);
    client.send(reply);
    std::cout << "Server sent PONG to " << client.get_nick() << std::endl;
    return Client::KEEP_CLIENT_SOCKET;
}