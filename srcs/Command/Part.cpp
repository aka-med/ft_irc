#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Utils.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

bool Command::part(Server &server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if (args.size() < 2)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "PART", "Not enough parameters");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    std::string channel_name = args[1];
    if (channel_name.empty() || channel_name[0] != '#')
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOSUCHCHANNEL, target, channel_name, "No such channel");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    
    if (!server.channelExists(channel_name))
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOSUCHCHANNEL, target, channel_name, "No such channel");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    
    Channel *channel = server.getChannel(channel_name);
    if (!channel->is_user_in_channel(client))
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOTONCHANNEL, target, channel_name, "You're not on that channel");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }

    reply = Reply::format_command(client, "PART", channel_name, "");
    channel->broadcast(reply, client);
    client->send(reply);
    server.removeClientFromChannel(channel_name, client);
    client->remove_channel_from_client(channel_name);
    
    return Client::KEEP_CLIENT_SOCKET;
}