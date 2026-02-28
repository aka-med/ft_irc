#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::kick(Server &server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if (args.size() < 3)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "KICK", "Not enough parameters");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    // KICK #channel_name user_to_kick :reason --- IGNORE ---
    std::string channel_name = args[1];
    std::string user_to_kick = args[2];
    std::string reason = args.size() > 3 ? Utils::join(args, 3, " ") : "No reason specified";
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
    if (channel->is_operator(client))
    {
        Client *user_client = server.getClientByNick(user_to_kick);
        if (!user_client)
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_NOSUCHNICK, target, user_to_kick, "No such nick");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        if (!channel->is_user_in_channel(user_client))
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_USERNOTINCHANNEL, target, user_to_kick + " " + channel_name, "They aren't on that channel");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        reply = Reply::format_command(client, "KICK", channel_name + " " + user_to_kick, reason);
        channel->broadcast(reply, client);
        client->send(reply);
        server.removeClientFromChannel(channel_name, user_client);
    }
    else
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_CHANOPRIVSNEEDED, target, channel_name, "You're not channel operator");
        client->send(reply);
    }
    return Client::KEEP_CLIENT_SOCKET;
}