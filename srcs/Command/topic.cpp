#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"

bool Command::topic(Server *server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;

    target = client->get_nick();
    if(args.size() < 2)
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NEEDMOREPARAMS, target, "TOPIC", "Not enough parameters");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    std::string channel_name = args[1];
    if(channel_name.empty() || channel_name[0] != '#' || !server->channelExists(channel_name))
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NOSUCHCHANNEL, target, channel_name, "No such channel");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    Channel *channel = server->getChannel(channel_name);
    if (!channel->is_user_in_channel(client))
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_NOTONCHANNEL, target, channel_name, "You're not on that channel");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (args.size() == 2)
    {
        if (channel->get_topic().empty())
            reply = Reply::format_reply(server->get_name(), Reply::RPL_NOTOPIC, target, channel_name, "No topic is set");
        else
            reply = Reply::format_reply(server->get_name(), Reply::RPL_TOPIC, target, channel_name, channel->get_topic());
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (channel->get_t_flag() && !channel->is_operator(client))
    {
        reply = Reply::format_reply(server->get_name(), Reply::ERR_CHANOPRIVSNEEDED, target, channel_name, "You're not channel operator");
        client->add_to_queue(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    std::string new_topic = Utils::join(args, 2, " ");
    channel->set_topic(new_topic);

    reply = Reply::format_command(client, "TOPIC", channel_name, new_topic);
    channel->broadcast(reply, client);
    client->add_to_queue(reply);

    return Client::KEEP_CLIENT_SOCKET;
}