#include "../../includes/Client.hpp"
#include "../../includes/Bot.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Utils.hpp"
bool Command::join(Server &server, Client *client, const std::vector<std::string> &args)
{
    std::string reply;
    std::string target;
    std::string channel_name;
    std::string users_list;
    Channel *channel = NULL;

    target = client->get_nick();
    if (args.size() < 2)
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NEEDMOREPARAMS, target, "JOIN", "Not enough parameters");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    channel_name = args[1];
    if (channel_name.empty() || channel_name[0] != '#')
    {
        reply = Reply::format_reply(server.get_name(), Reply::ERR_NOSUCHCHANNEL, target, channel_name, "No such channel");
        client->send(reply);
        return Client::KEEP_CLIENT_SOCKET;
    }
    if (!server.channelExists(channel_name))
    {
        server.addClientToChannel(channel_name, client, Server::FIRST_CHANNEL_JOIN);
        client->add_channel_to_client(channel_name);
        channel = server.getChannel(channel_name);
        
        reply = Reply::format_command(client, "JOIN", channel_name, "");
        client->send(reply);

        reply = Reply::format_reply(server.get_name(), Reply::RPL_NOTOPIC, target, channel_name, "No topic is set");
        client->send(reply);
    }
    else
    {
        channel = server.getChannel(channel_name);
        if (!channel)
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_NOSUCHCHANNEL, target, channel_name, "No such channel");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        if (channel->is_user_in_channel(client))
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_USERONCHANNEL, target, channel_name, "You are already on that channel");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        if (channel->get_i_flag())
        {
            if (!channel->is_user_invited(client))
            {
                reply = Reply::format_reply(server.get_name(), Reply::ERR_INVITEONLYCHAN, target, channel_name, "Cannot join channel (+i)");
                client->send(reply);
                return Client::KEEP_CLIENT_SOCKET;
            }
            channel->remove_invited_user(client);
        }
        if (channel->get_l_flag() && channel->get_users_count() >= static_cast<size_t>(channel->get_limit()))
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_CHANNELISFULL, target, channel_name, "Cannot join channel (+l)");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        if (channel->get_k_flag() && (args.size() < 3 || !channel->check_key(args[2])))
        {
            reply = Reply::format_reply(server.get_name(), Reply::ERR_BADCHANNELKEY, target, channel_name, "Cannot join channel (+k)");
            client->send(reply);
            return Client::KEEP_CLIENT_SOCKET;
        }
        server.addClientToChannel(channel_name, client, Server::OTHER_CHANNEL_JOIN);
        client->add_channel_to_client(channel_name);
        reply = Reply::format_command(client, "JOIN", channel_name, "");
        channel->broadcast(reply, client);
        client->send(reply);

        if (channel->get_topic().empty())
            client->send(Reply::format_reply(server.get_name(), Reply::RPL_NOTOPIC, target, channel_name, "No topic is set"));
        else
            client->send(Reply::format_reply(server.get_name(), Reply::RPL_TOPIC, target, channel_name, channel->get_topic()));
    }
    users_list = channel->get_users_list_names();
    reply = Reply::format_reply(server.get_name(), Reply::RPL_NAMREPLY, target, "= " + channel_name, users_list);
    client->send(reply);
    reply = Reply::format_reply(server.get_name(), Reply::RPL_ENDOFNAMES, target, channel_name, "End of /NAMES list");
    client->send(reply);
    return Client::KEEP_CLIENT_SOCKET;
}