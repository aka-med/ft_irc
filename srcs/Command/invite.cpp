#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::invite(Server &server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if(args.size() < 3)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "INVITE", "Not enough parameters");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    std::string channel_name = args[2];
    if(channel_name.empty() || channel_name[0] != '#' || !server.channelExists(channel_name))
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
    std::string nickname = args[1];
    Client *trgclient = server.getClientByNick(nickname);
    if(!trgclient)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOSUCHNICK, target, nickname, "No such nick");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if(channel->is_user_in_channel(trgclient))
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_USERONCHANNEL, target, nickname + " " + channel_name, "is already on channel");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if(channel->get_i_flag() && !channel->is_operator(client))
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_CHANOPRIVSNEEDED, target, channel_name, "You're not channel operator");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    reply = Reply::format_reply(server.get_name(), Reply::RPL_INVITING, target, nickname, channel_name);
    client->send(reply);
    channel->add_invited_user(trgclient);
    reply = Reply::format_command(client, "INVITE", nickname, channel_name);
    trgclient->send(reply);
    return Client::KEEP_CLIENT_SOCKET;
}
