#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"
#include "../includes/Reply.hpp"
#include "../includes/Utils.hpp"

bool Command::join(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client.get_nick().empty() ? "*" : client.get_nick();
    if (!client.is_authenticated())
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOTREGISTERED, target, "", "You have not registered");
        client.send(reply);
        return Client::REMOVE_CLIENT_SOCKET;
    }
    if (args.size() < 2)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "JOIN", "Not enough parameters");
        client.send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    // here you should handel the join command and add the client to the channel
    // if the channel does not exist create it and add the client to it
    // if the channel exist add the client to it
    // you should also check if the channel is full or not and if the key is correct or not
    return Client::KEEP_CLIENT_SOCKET;
}